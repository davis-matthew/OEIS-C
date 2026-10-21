/*
 * A083729  Decimal expansion of sqrt(2)/(sqrt(2)-1)^2 = 3*sqrt(2)+4.
 * https://oeis.org/A083729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, nonn
 * Formula:   a(n) = decimal digits of 3*sqrt(2)+4 with a(1)=8 * The sequence
 *            is 8,2,4,2,6,4,0,6,8,7,1,1,9,2,8,5,1,4,6,4,0,5,0,6,... * *
 *            Computation is performed by the digit‑by‑digit algorithm for the
 *            * square root of 18 (since 3*sqrt(2)+4 = sqrt(18)+4). * * For
 *            arbitrary‑precision builds seq_t is a big‑integ...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* --------------------------------------------------------------------
 *   Lightweight big‑integer helper (32‑bit words) – used only for the
 *   intermediate remainder and root during digit generation.  It is
 *   independent of seq_t, so it works for both 64‑bit and arbitrary‑
 *   precision builds.
 * -------------------------------------------------------------------- */
typedef struct {
    size_t len;          /* number of words used (little‑endian) */
    size_t cap;          /* capacity in words */
    uint32_t *w;         /* array of 32‑bit words */
} BigInt;

/* initialise an empty BigInt */
static void bi_init(BigInt *b) {
    b->len = 0;
    b->cap = 0;
    b->w   = NULL;
}

/* ensure capacity for at least `need` words */
static void bi_ensure(BigInt *b, size_t need) {
    if (b->cap < need) {
        size_t newcap = need * 2;
        if (newcap < 2) newcap = 2;
        uint32_t *p = (uint32_t *)realloc(b->w, newcap * sizeof(uint32_t));
        if (!p) abort();                 /* allocation failure */
        b->w = p;
        b->cap = newcap;
    }
}

/* copy src to dst (dest is allowed to be the same as src) */
static void bi_copy(BigInt *dst, const BigInt *src) {
    bi_ensure(dst, src->len);
    dst->len = src->len;
    for (size_t i = 0; i < src->len; ++i)
        dst->w[i] = src->w[i];
}

/* set BigInt to zero (keeps the current capacity) */
static void bi_zero(BigInt *b) {
    b->len = 0;
}

/* set BigInt to a small uint64_t value */
static void bi_from_uint(BigInt *b, uint64_t v) {
    bi_zero(b);
    while (v) {
        bi_ensure(b, b->len + 1);
        b->w[b->len++] = (uint32_t)(v & 0xffffffffu);
        v >>= 32;
    }
}

/* multiply by a small 32‑bit integer (in place) */
static void bi_mul_small(BigInt *a, uint32_t m) {
    if (m == 0) { bi_zero(a); return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->w[i] * m + carry;
        a->w[i] = (uint32_t)(prod & 0xffffffffu);
        carry   = prod >> 32;
    }
    if (carry) {
        bi_ensure(a, a->len + 1);
        a->w[a->len++] = (uint32_t)carry;
    }
}

/* add a small unsigned 32‑bit integer (in place) */
static void bi_add_uint(BigInt *a, uint32_t v) {
    uint64_t sum = (uint64_t)(a->len ? a->w[0] : 0) + v;
    size_t i = 0;
    while (1) {
        if (i >= a->len) {
            bi_ensure(a, i + 1);
            a->w[i] = (uint32_t)(sum & 0xffffffffu);
            a->len = i + 1;
            break;
        }
        sum = (uint64_t)a->w[i] + v;
        a->w[i] = (uint32_t)(sum & 0xffffffffu);
        if ((sum >> 32) == 0) break;
        v = 0;
        ++i;
    }
}

/* compare two BigInts: return 1 if a>b, 0 if equal, -1 if a<b */
static int bi_cmp(const BigInt *a, const BigInt *b) {
    if (a->len > b->len) return 1;
    if (a->len < b->len) return -1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->w[i] > b->w[i]) return 1;
        if (a->w[i] < b->w[i]) return -1;
    }
    return 0;
}

/* subtract b from a (a must be >= b). Result stored in a. */
static void bi_sub(BigInt *a, const BigInt *b) {
    uint64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t subtrahend = (uint64_t)(i < b->len ? b->w[i] : 0) + borrow;
        if (a->w[i] >= subtrahend) {
            a->w[i] = (uint32_t)(a->w[i] - subtrahend);
            borrow = 0;
        } else {
            a->w[i] = (uint32_t)(0x100000000ull + a->w[i] - subtrahend);
            borrow = 1;
        }
    }
    /* strip leading zeros */
    while (a->len > 0 && a->w[a->len - 1] == 0) --a->len;
}

/* --------------------------------------------------------------------
 *   Digit generation for sqrt(18)
 * -------------------------------------------------------------------- */
static BigInt root_big;          /* current root (as big int) */
static BigInt rem_big;           /* current remainder (as big int) */
static BigInt temp_big;          /* temporary big int reused each step */
static int bs_init_done = 0;     /* flag to initialise once */

/* dynamic array of decimal digits of sqrt(18) (fractional part) */
static seq_t *dig_arr  = NULL;
static size_t dig_cap  = 0;
static size_t dig_len  = 0;

/* ensure we have at least `need` digits in dig_arr (0‑based) */
static void ensure_digits(size_t need) {
    if (!bs_init_done) {
        /* initialise sqrt(18) : integer part 4, remainder 2 */
        bi_zero(&root_big); bi_zero(&rem_big); bi_zero(&temp_big);
        bi_from_uint(&root_big, 4);   /* 4^2 = 16 <= 18 */
        bi_from_uint(&rem_big, 2);    /* 18 - 4^2 */
        bs_init_done = 1;
    }

    if (dig_arr == NULL) {
        dig_cap = (need > 32 ? need : 32);
        dig_arr = (seq_t *)malloc(dig_cap * sizeof(seq_t));
        if (!dig_arr) abort();
    } else if (dig_cap < need) {
        size_t newcap = dig_cap * 2;
        if (newcap < need) newcap = need;
        seq_t *tmp = (seq_t *)realloc(dig_arr, newcap * sizeof(seq_t));
        if (!tmp) abort();
        dig_arr = tmp;
        dig_cap = newcap;
    }

    while (dig_len < need) {
        /* bring down two zeroes: rem = rem * 100 */
        bi_mul_small(&rem_big, 100);

        /* find largest digit d such that (20*root + d)*d <= rem */
        int d = 0;
        for (int i = 9; i >= 0; --i) {
            bi_copy(&temp_big, &root_big);        /* temp = root */
            bi_mul_small(&temp_big, 20);          /* temp = 20*root */
            bi_add_uint(&temp_big, i);           /* temp = 20*root + i */
            bi_mul_small(&temp_big, i);          /* temp = (20*root + i)*i */

            if (bi_cmp(&temp_big, &rem_big) <= 0) {
                d = i;
                bi_sub(&rem_big, &temp_big);     /* rem -= product */
                break;
            }
        }

        /* store digit and update root */
        dig_arr[dig_len++] = (seq_t)d;
        bi_mul_small(&root_big, 10);
        bi_add_uint(&root_big, d);
    }
}

/* --------------------------------------------------------------------
 *   Public sequence function
 * -------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 1) return (seq_t)8;          /* integer part of 3*sqrt(2)+4 */
    /* for n >= 2 the (n-2)-th fractional digit of sqrt(18) is needed */
    size_t need = (size_t)(n - 1);        /* because dig_arr[0] = a(2) */
    ensure_digits(need);
    return dig_arr[n - 2];
}
