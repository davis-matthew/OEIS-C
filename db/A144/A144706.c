/*
 * A144706  Central coefficients of the triangle A132047.
 * https://oeis.org/A144706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A144706: Central coefficients of triangle A132047.
   Formula: a(n) = 3*binomial(2*n, n) – 2*0^n   (so a(0)=1, a(n>=1)=3*binomial(2*n,n))
 */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Special case: n == 0 -> a(0) = 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* If seq_t fits into 64 bits, compute the binomial using reduction
       to avoid intermediate overflow.  Otherwise, delegate to binomial(). */
    if (sizeof(seq_t) <= 8) {
        /* Allocate array for numerator factors (n+1 .. 2n) */
        seq_t *nums = (seq_t *)calloc((size_t)n, sizeof(seq_t));
        if (!nums) return 0; /* allocation failure → 0 as fallback */

        for (idx_t i = 0; i < n; ++i) {
            nums[i] = (seq_t)(n + 1 + i); /* values n+1 .. 2n */
        }

        /* Reduce denominators 2 .. n against the numerators */
        for (idx_t d = 2; d <= n; ++d) {
            seq_t rem = (seq_t)d;
            for (idx_t j = 0; j < n && rem > 1; ++j) {
                seq_t g = gcd(nums[j], rem);
                if (g > 1) {
                    nums[j] /= g;
                    rem   /= g;
                }
            }
        }

        /* Multiply the remaining factors -> binomial(2n,n) */
        seq_t bin = 1;
        for (idx_t i = 0; i < n; ++i) {
            bin *= nums[i];
        }

        free(nums);
        return bin * 3;                  /* a(n) for n>0 */
    } else {
        /* For 128‑bit or arbitrary precision builds, use the provided
           binomial routine which handles large intermediate values. */
        seq_t bin = binomial(2 * n, n);
        return bin * 3;                  /* a(n) for n>0 */
    }
}
