/*
 * A037286  Replace 2n+1 with concatenation of its divisors.
 * https://oeis.org/A037286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenation of the divisors of (2n+1) in ascending
 *            order
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   digitlen, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/pow10.h"     /* seq_t pow10(idx_t e) – returns 10^e as seq_t */
#include "../utils/digitlen.h"  /* static int digitlen(uint64_t x) – number of decimal digits */

/* Compute the nth term of OEIS A037286. */
seq_t a(idx_t n)
{
    /* M = 2n+1 (in seq_t for correctness, but we use idx_t for division). */
    idx_t M = (idx_t)n * 2 + 1;

    /* Find floor sqrt(M) without overflow. */
    idx_t sqrtM = 0;
    for (idx_t i = 1; i <= M / i; ++i) sqrtM = i;

    /* Allocate temporary arrays for small and large divisors.
       For values up to 2^64, sqrtM <= 2^32, so the allocation is safe for all tests
       that are intended.  */
    idx_t *small = (idx_t *)malloc((sqrtM + 1) * sizeof(idx_t));
    idx_t *large = (idx_t *)malloc((sqrtM + 1) * sizeof(idx_t));
    if (!small || !large) {
        /* Allocation failed: return 0 (undefined, but keeps the compiler happy). */
        return (seq_t)0;
    }
    size_t sc = 0, lc = 0;

    /* Gather divisors. */
    for (idx_t d = 1; d <= sqrtM; ++d) {
        if (M % d == 0) {
            small[sc++] = d;
            if (d != M / d) {
                large[lc++] = M / d;
            }
        }
    }

    /* Concatenate divisors: small ascending, then large ascending (reverse order). */
    seq_t result = 0;
    for (size_t i = 0; i < sc; ++i) {
        seq_t d = (seq_t)small[i];
        int dlen = digitlen((uint64_t)d);
        result = result * pow10(dlen) + d;
    }
    for (size_t i = 0; i < lc; ++i) {
        seq_t d = (seq_t)large[lc - 1 - i];
        int dlen = digitlen((uint64_t)d);
        result = result * pow10(dlen) + d;
    }

    free(small);
    free(large);
    return result;
}
