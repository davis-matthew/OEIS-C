/*
 * A133393  Start with a(1)=1; for n >= 1, a(n+1)=a(n)+a(k) with k=[n - n-th
 *          digit of sqrt(2)]. If k<0 or k=0, then a(k)=0.
 * https://oeis.org/A133393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1) = 1 a(n+1) = a(n) + a(k) where k = n - d(n) and d(n) = the
 *            n‑th digit of sqrt(2) (if k <= 0 then a(k) = 0)
 * Range:     u8 n <= 21, u16 n <= 41, u32 n <= 80, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Big‑integer base (decimal digits in base 1e9) */
#define BIGINT_BASE 1000000000U

/* Simple big‑integer structure */
typedef struct {
    uint32_t *d;   /* limbs (least significant first) */
    size_t   len;  /* number of limbs used */
    size_t   cap;  /* allocated limb capacity */
} BigInt;

/* Grow a BigInt to hold at least 'needed' limbs */
static void big_grow(BigInt *b, size_t needed) {
    if (b->cap < needed) {
        size_t newcap = b->cap ? b->cap * 2 : 64;
        while (newcap < needed) newcap *= 2;
        b->d = (uint32_t *)realloc(b->d, newcap * sizeof(uint32_t));
        b->cap = newcap;
    }
}

/* Copy a BigInt into another (dest) */
static void big_set(BigInt *dest, const BigInt *src) {
    big_grow(dest, src->len);
    memcpy(dest->d, src->d, src->len * sizeof(uint32_t));
    dest->len = src->len;
}

/* Multiply a BigInt by a small integer (m <= 100) into dest */
static void big_mul_small(BigInt *dest, const BigInt *src, uint32_t m) {
    size_t need = src->len + 1;            /* carry one limb at most */
    big_grow(dest, need);
    uint64_t carry = 0;
    for (size_t i = 0; i < src->len; ++i) {
        uint64_t prod = (uint64_t)src->d[i] * m + carry;
        dest->d[i] = (uint32_t)(prod % BIGINT_BASE);
        carry      = prod / BIGINT_BASE;
    }
    if (carry) {
        dest->d[src->len] = (uint32_t)carry;
        dest->len = src->len + 1;
    } else {
        dest->len = src->len;
    }
}

/* Add a small integer (add <= 9) to a BigInt in place */
static void big_add_small(BigInt *dst, uint32_t add) {
    size_t i;
    uint64_t carry = add;
    for (i = 0; i < dst->len && carry; ++i) {
        uint64_t sum = (uint64_t)dst->d[i] + carry;
        dst->d[i]   = (uint32_t)(sum % BIGINT_BASE);
        carry       = sum / BIGINT_BASE;
    }
    if (carry) {
        big_grow(dst, dst->len + 1);
        dst->d[dst->len] = (uint32_t)carry;
        dst->len++;
    }
}

/* Compare two BigInts: return -1,0,1 */
static int big_cmp(const BigInt *a, const BigInt *b) {
    if (a->len != b->len) return (a->len < b->len) ? -1 : 1;
    for (size_t i = a->len; i-- > 0;) {
        if (a->d[i] != b->d[i])
            return (a->d[i] < b->d[i]) ? -1 : 1;
    }
    return 0;
}

/* Subtract b from a (a >= b) in place */
static void big_sub_inplace(BigInt *a, const BigInt *b) {
    size_t i;
    int64_t borrow = 0;
    for (i = 0; i < a->len; ++i) {
        int64_t diff = (int64_t)a->d[i] - (i < b->len ? b->d[i] : 0) - borrow;
        if (diff < 0) {
            diff += BIGINT_BASE;
            borrow = 1;
        } else
            borrow = 0;
        a->d[i] = (uint32_t)diff;
    }
    while (a->len > 1 && a->d[a->len - 1] == 0)
        --a->len;
}

/* Globals for sqrt(2) digit generation */
static uint8_t *sqrt_digits = NULL;
static size_t sqrt_digits_len = 0;

static BigInt root, rem, tmp, left;

/* Ensure zero initialization of BigInts */
static void init_sqrt(void) {
    root.d = root.d ? root.d : NULL; root.len = root.cap = 0;
    rem.d = rem.d ? rem.d : NULL; rem.len = rem.cap = 0;
    tmp.d = tmp.d ? tmp.d : NULL; tmp.len = tmp.cap = 0;
    left.d = left.d ? left.d : NULL; left.len = left.cap = 0;
}

/* Return the n-th digit of sqrt(2) (1‑based). */
static uint8_t sqrt_digit(idx_t n) {
    while (sqrt_digits_len < (size_t)n) {
        if (sqrt_digits_len == 0) { /* first digit (integer part) */
            big_grow(&root, 1);
            root.d[0] = 1;
            root.len  = 1;
            big_grow(&rem, 1);
            rem.d[0] = 1;          /* 2 - 1^2 = 1 */
            rem.len  = 1;
            sqrt_digits = (uint8_t *)realloc(sqrt_digits, 64);
            sqrt_digits[0] = 1;
            sqrt_digits_len = 1;
            continue;
        }

        /* bring down two zeros: R = R * 100 */
        big_mul_small(&rem, &rem, 100);

        int best = 0;
        for (int d = 9; d >= 0; --d) {
            /* tmp = root * 20 */
            big_mul_small(&tmp, &root, 20);
            /* tmp += d */
            big_add_small(&tmp, (uint32_t)d);
            /* left = tmp * d */
            big_mul_small(&left, &tmp, (uint32_t)d);
            if (big_cmp(&left, &rem) <= 0) {
                best = d;
                break;
            }
        }

        /* R -= left */
        big_sub_inplace(&rem, &left);

        /* root = root * 10 + best */
        big_mul_small(&tmp, &root, 10);
        big_add_small(&tmp, (uint32_t)best);
        big_set(&root, &tmp);

        /* Store digit */
        uint8_t *newbuf = (uint8_t *)realloc(sqrt_digits,
                                            sqrt_digits_len + 64);
        sqrt_digits = newbuf;
        sqrt_digits[sqrt_digits_len++] = (uint8_t)best;
    }
    return sqrt_digits[n - 1];
}

/* Cache for the sequence */
static seq_t *cache = NULL;
static size_t cache_size = 0, cache_cap = 0;

/* The required function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* out of bounds – not used */

    /* One‑time initialization */
    if (cache_size == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;       /* a(1) = 1 */
        cache_size = 1;
        init_sqrt();               /* initialise sqrt state */
    }

    /* Compute terms up to n */
    while (cache_size < (size_t)n) {
        idx_t curr = (idx_t)cache_size;             /* n-1 value */
        uint8_t d = sqrt_digit(curr);               /* d(curr) */
        idx_t k = (curr >= d) ? curr - d : 0;
        seq_t ak = (k == 0) ? (seq_t)0 : cache[k - 1];
        seq_t next = cache[cache_size - 1] + ak;

        if (cache_size == cache_cap) {
            size_t newcap = cache_cap * 2;
            cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            cache_cap = newcap;
        }
        cache[cache_size] = next;
        ++cache_size;
    }
    return cache[n - 1];
}
