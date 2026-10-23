/*
 * A010505  Decimal expansion of square root of 52.
 * https://oeis.org/A010505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   decimal digits of √52 (a(1) is the integer part)
 * Range:     u8 n <= 9862, u16 n <= 9862, u32 n <= 9862, u64 n <= 9862,
 *            u128 n <= 9862, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Big integer arithmetic limited to positive integers only.
 *  Base = 1e9 for compact storage.  Capacity is large enough for
 *  the required number of digits (ceil(10000/9)+1 ≈ 1112).  */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef uint32_t bint_t[2500];   /* 2500 * 1e9 ≈ 2.5×10^12 digits, ample for 10k decimal places */

static const uint32_t BASE = 1000000000U;

/* Big integer structure */
typedef struct {
    bint_t d;      /* little‑endian digits, d[0] least significant */
    size_t len;    /* number of used digits (len==0 means 0) */
} BigInt;

/* Initialise to zero */
static void bi_zero(BigInt *a) {
    a->len = 0;
}

/* Initialise from uint32_t value */
static void bi_set_uint(BigInt *a, uint32_t v) {
    if (v == 0) {
        a->len = 0;
        return;
    }
    a->d[0] = v;
    a->len = 1;
}

/* Copy big integer */
static void bi_copy(BigInt *dst, const BigInt *src) {
    dst->len = src->len;
    for (size_t i = 0; i < src->len; ++i)
        dst->d[i] = src->d[i];
}

/* Trim leading zero digits */
static void bi_trim(BigInt *a) {
    while (a->len > 0 && a->d[a->len - 1] == 0)
        a->len--;
}

/* Multiply by small integer m (<= 100 here) */
static void bi_mul_small(BigInt *a, uint32_t m) {
    if (m == 0 || a->len == 0) {
        a->len = 0;
        return;
    }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint32_t)(prod % BASE);
        carry = prod / BASE;
    }
    if (carry) {
        a->d[a->len] = (uint32_t)carry;
        a->len++;
    }
}

/* Add small integer v (v < BASE) to the big integer */
static void bi_add_small(BigInt *a, uint32_t v) {
    if (v == 0) return;
    uint64_t sum = (uint64_t)(a->len ? a->d[0] : 0) + v;
    if (a->len == 0) {
        a->d[0] = (uint32_t)(sum % BASE);
        if (sum >= BASE) a->len = 2, a->d[1] = 1;
        else a->len = 1;
        return;
    }
    a->d[0] = (uint32_t)(sum % BASE);
    uint64_t carry = sum / BASE;
    size_t i = 1;
    while (carry) {
        if (i < a->len) {
            uint64_t s = (uint64_t)a->d[i] + carry;
            a->d[i] = (uint32_t)(s % BASE);
            carry = s / BASE;
        } else {
            a->d[i] = (uint32_t)carry;
            carry = 0;
        }
        ++i;
    }
    a->len = (i > a->len) ? i : a->len;
}

/* Compare two big integers: return -1 if a<b, 0 if a==b, 1 if a>b */
static int bi_cmp(const BigInt *a, const BigInt *b) {
    if (a->len != b->len)
        return (a->len < b->len) ? -1 : 1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->d[i] != b->d[i])
            return (a->d[i] < b->d[i]) ? -1 : 1;
    }
    return 0;
}

/* Subtract b from a (assume a >= b) */
static void bi_sub_eq(BigInt *a, const BigInt *b) {
    uint64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t sub = (uint64_t)(i < b->len ? b->d[i] : 0) + borrow;
        if (a->d[i] >= sub) {
            a->d[i] = (uint32_t)(a->d[i] - sub);
            borrow = 0;
        } else {
            a->d[i] = (uint32_t)(BASE + a->d[i] - sub);
            borrow = 1;
        }
    }
    bi_trim(a);
}

/* --------------------------------------------------------------------- */
/*  Sequence generation code                                            */
/* --------------------------------------------------------------------- */

/* Static cache for already generated digits */
static int *digit_cache = NULL;      /* array of digits 0..9 */
static size_t cache_len = 0;         /* number of digits cached */
static size_t cache_cap = 0;         /* allocated capacity of digit_cache */

/* Big integers for ongoing computation */
static BigInt rem;
static BigInt cur;

/* Ensure the cache can hold at least n digits */
static void ensure_cache(size_t n) {
    if (cache_cap >= n) return;
    size_t new_cap = (cache_cap == 0) ? (n > 1000 ? n : 1000) : cache_cap * 2;
    if (new_cap < n) new_cap = n;
    digit_cache = (int *)realloc(digit_cache, new_cap * sizeof(int));
    cache_cap = new_cap;
}

/* Generate digits up to position 'n' (1‑based) */
static void generate_up_to(size_t n) {
    if (n <= cache_len) return;

    if (cache_len == 0) {   /* first run */
        bi_zero(&rem);
        bi_zero(&cur);
    }

    for (size_t pos = cache_len; pos < n; ++pos) {
        /* Bring next pair */
        uint32_t pair = (pos == 0) ? 52U : 0U;
        bi_mul_small(&rem, 100U);      /* rem = rem * 100 */
        bi_add_small(&rem, pair);      /* rem += pair */

        /* Compute p = cur * 20 */
        BigInt p;
        bi_copy(&p, &cur);
        bi_mul_small(&p, 20U);

        /* Find largest d such that (p + d) * d <= rem */
        int d_found = 0;
        for (int d = 9; d >= 0; --d) {
            BigInt cand;
            bi_copy(&cand, &p);           /* cand = p */
            bi_mul_small(&cand, (uint32_t)d);     /* cand = p * d */
            bi_add_small(&cand, (uint32_t)(d * d)); /* cand += d*d */

            if (bi_cmp(&cand, &rem) <= 0) {
                d_found = d;
                /* rem -= cand */
                bi_sub_eq(&rem, &cand);
                /* cur = cur * 10 + d */
                bi_mul_small(&cur, 10U);
                bi_add_small(&cur, (uint32_t)d);
                break;
            }
        }
        digit_cache[cache_len++] = d_found;
    }
}

/* --------------------------------------------------------------------- */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined offset, return 0 */

    size_t idx = (size_t)n;        /* safe cast: n >= 1 */

    if (idx > cache_cap) {
        ensure_cache(idx);
    }
    generate_up_to(idx);
    return (seq_t)digit_cache[idx - 1];
}
