/*
 * A204632  Least k such that n*k!! contains every digit at least once.
 * https://oeis.org/A204632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the least k such that n * k!! contains all decimal digits 0–9 at least once. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*--------------------------------------------------------------------
 * Helper: big decimal number stored as decimal digits (little endian)
 *--------------------------------------------------------------------*/
typedef struct {
    uint8_t *d;    /* digits 0–9, d[0] least significant */
    size_t len;    /* number of used digits */
    size_t cap;    /* allocated capacity */
} BigDec;

/* Ensure capacity for at least need digits. */
static inline void bigdec_reserve(BigDec *a, size_t need) {
    if (a->cap < need) {
        size_t newcap = a->cap ? a->cap * 2 : 16;
        while (newcap < need) newcap <<= 1;
        uint8_t *tmp = (uint8_t *)realloc(a->d, newcap);
        if (!tmp) exit(1);          /* out of memory: abort */
        a->d   = tmp;
        a->cap = newcap;
    }
}

/* Initialize BigDec to 1. */
static inline void bigdec_init_one(BigDec *a) {
    a->cap = 16;
    a->d   = (uint8_t *)malloc(a->cap);
    if (!a->d) exit(1);
    a->len   = 1;
    a->d[0]  = 1;
}

/* Free BigDec memory. */
static inline void bigdec_free(BigDec *a) {
    free(a->d);
    a->d   = NULL;
    a->len = a->cap = 0;
}

/* Copy src into dst (dst is resized as needed). */
static inline void bigdec_copy(BigDec *dst, const BigDec *src) {
    dst->len = src->len;
    bigdec_reserve(dst, src->len);
    memcpy(dst->d, src->d, src->len);
}

/* Multiply big decimal number by small integer m (m fits in uint64_t). */
static inline void bigdec_mul_small(BigDec *a, uint64_t m) {
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint8_t)(prod % 10);
        carry   = prod / 10;
    }
    while (carry) {
        bigdec_reserve(a, a->len + 1);
        a->d[a->len++] = (uint8_t)(carry % 10);
        carry /= 10;
    }
}

/*--------------------------------------------------------------------
 * Main sequence function
 *--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* offset starts at 1; return 0 for n==0 (unused). */
    if (n == 0) return 0;

    const uint64_t n64 = (uint64_t)n;     /* small multiplier */

    /* Big decimal representations of current even and odd double factorials. */
    BigDec prod_even, prod_odd;
    bigdec_init_one(&prod_even);
    bigdec_init_one(&prod_odd);

    /* Temporary buffer for n * (current double factorial). */
    BigDec temp;
    bigdec_init_one(&temp);               /* will be overwritten per iteration */

    /* Main search loop. Safe infinite loop; k always exists. */
    for (idx_t k = 1; ; ++k) {
        /* Update the appropriate double factorial product. */
        if ((k & 1u) == 0) {
            bigdec_mul_small(&prod_even, (uint64_t)k);
        } else {
            bigdec_mul_small(&prod_odd, (uint64_t)k);
        }

        /* Choose the current parity product. */
        BigDec *src = ((k & 1u) == 0) ? &prod_even : &prod_odd;

        /* temp = src * n64 */
        bigdec_copy(&temp, src);
        bigdec_mul_small(&temp, n64);

        /* Determine whether all digits 0–9 appear. */
        unsigned char seen[10] = {0};
        size_t distinct = 0;
        for (size_t i = 0; i < temp.len; ++i) {
            unsigned int d = temp.d[i];
            if (!seen[d]) {
                seen[d] = 1;
                if (++distinct == 10) break;
            }
        }

        if (distinct == 10) {
            /* Clean up and return the answer. */
            bigdec_free(&prod_even);
            bigdec_free(&prod_odd);
            bigdec_free(&temp);
            return (seq_t)k;
        }
    }

    /* Unreachable, but keep compiler happy. */
    return (seq_t)0;
}
