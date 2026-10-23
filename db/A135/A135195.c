/*
 * A135195  Numbers n that raised to the powers from 1 to k (with k>=1) are
 *          multiple of the sum of their digits (n raised to k+1 must not be a
 *          multiple). Case k=10.
 * https://oeis.org/A135195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer m such that m^1 , m^2 , … , m^10
 *            are all divisible by the sum of the decimal digits of the
 *            corresponding power, while m^11 is not divisible by the sum of
 *            its digits. The function uses a small on‑the‑fly
 *            arbitrary‑precision implementation (decimal words of base
 *            10^9...
 * Range:     u8 n <= 1, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define BIG_BASE      1000000000U   /* 10^9, each word stores 9 decimal digits */
#define BIG_MAX_WORDS 128           /* enough for n^10 where n < 10^12 */

/* Simple bigint for decimal words */
typedef struct {
    uint32_t d[BIG_MAX_WORDS];
    size_t  len;
} BigInt;

/* Initialise to zero */
static inline void big_init(BigInt *b) {
    b->len = 0;
}

/* Set to an unsigned 64-bit integer */
static inline void big_set_uint64(BigInt *b, uint64_t x) {
    big_init(b);
    while (x) {
        b->d[b->len++] = (uint32_t)(x % BIG_BASE);
        x /= BIG_BASE;
    }
    if (b->len == 0)
        b->len = 1, b->d[0] = 0;
}

/* Multiply bigint by an unsigned 64-bit integer */
static inline void big_mul_uint64(BigInt *b, uint64_t m) {
    if (m == 0) {
        big_set_uint64(b, 0);
        return;
    }
    unsigned __int128 carry = 0;
    for (size_t i = 0; i < b->len; ++i) {
        unsigned __int128 prod = (unsigned __int128)b->d[i] * m + carry;
        b->d[i] = (uint32_t)(prod % BIG_BASE);
        carry   = prod / BIG_BASE;
    }
    while (carry) {
        b->d[b->len++] = (uint32_t)(carry % BIG_BASE);
        carry /= BIG_BASE;
    }
    /* Trim leading zeros */
    while (b->len > 1 && b->d[b->len-1] == 0)
        --b->len;
}

/* Compute sum of decimal digits of the bigint */
static inline uint64_t big_sum_digits(const BigInt *b) {
    uint64_t sum = 0;
    for (size_t i = 0; i < b->len; ++i) {
        uint32_t v = b->d[i];
        while (v) {
            sum += v % 10;
            v   /= 10;
        }
    }
    return sum;
}

/* Compute b mod m where m fits in 64‑bit */
static inline uint64_t big_mod(const BigInt *b, uint64_t mod) {
    uint64_t r = 0;
    for (long i = (long)b->len - 1; i >= 0; --i) {
        r = (r * BIG_BASE + b->d[i]) % mod;
    }
    return r;
}

/* Static cache of found terms */
#define CACHE_CAP 20000   /* sufficient for first 10 000 terms */
static seq_t cached[CACHE_CAP];
static idx_t  cached_count = 0;

/* Return the nth term (1‑based) */
seq_t a(idx_t n) {
    static idx_t last_checked = 0;          /* last candidate tested */
    /* Expand cache until we have at least n terms */
    while (cached_count < n) {
        const seq_t cand = (seq_t)(last_checked + 1);
        ++last_checked;                     /* next candidate */

        BigInt pow;                         /* current power */
        big_set_uint64(&pow, 1);            /* 1 = cand^0 */

        bool ok = true;
        /* Test powers 1 … 10 */
        for (idx_t exp = 1; exp <= 10; ++exp) {
            big_mul_uint64(&pow, (uint64_t)cand);   /* pow *= cand */
            uint64_t sum = big_sum_digits(&pow);
            if (sum == 0 || big_mod(&pow, sum) != 0) {
                ok = false;
                break;
            }
        }

        /* If all 10 powers passed, check the 11th */
        if (ok) {
            big_mul_uint64(&pow, (uint64_t)cand);   /* cand^11 */
            uint64_t sum11 = big_sum_digits(&pow);
            if (sum11 != 0 && big_mod(&pow, sum11) == 0)
                ok = false;    /* 11th power also works – reject */
        }

        if (ok) {
            if (cached_count < CACHE_CAP)
                cached[cached_count++] = cand;
        }
    }

    return cached[n-1];
}
