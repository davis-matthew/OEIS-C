/*
 * A091962  From enumerating paths in the plane.
 * https://oeis.org/A091962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n+6, 7)*(2*n+3)*(n+1)*(n+2)/240
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 19, u64 n <= 195, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Compute the 10‑th term of OEIS A091962.
   The implementation uses only arithmetic on seq_t, which may be a
   native integer type or an arbitrary‑precision integer class.
   It cancels the denominator 2^8·3^3·5^2·7 from the product
   of the 10 numerator factors, thus avoiding overflow for the
   subrange of n where the result fits in seq_t. */
seq_t a(idx_t n)
{
    /* The formula yields 0 for n = 0 (binomial(6,7) = 0). */
    seq_t nums[10];

    /* Convert arithmetic on idx_t to seq_t before use. */
    nums[0] = (seq_t)(2*n + 6); /* 2n + 6 */
    nums[1] = (seq_t)(2*n + 5); /* 2n + 5 */
    nums[2] = (seq_t)(2*n + 4); /* 2n + 4 */
    nums[3] = (seq_t)(2*n + 3); /* 2n + 3 (first of the extra factors) */
    nums[4] = (seq_t)(2*n + 2); /* 2n + 2 */
    nums[5] = (seq_t)(2*n + 1); /* 2n + 1 */
    nums[6] = (seq_t)(2*n);     /* 2n   */
    nums[7] = (seq_t)(2*n + 3); /* 2n + 3 (second of the extra factors) */
    nums[8] = (seq_t)(n + 1);   /* n + 1 */
    nums[9] = (seq_t)(n + 2);   /* n + 2 */

    /* Denominator equals 2^8·3^3·5^2·7   ( = 7!·240 ). */
    const int primes[4] = {2, 3, 5, 7};
    const int exps[4]   = {8, 3, 2, 1};

    /* Cancel each prime factor from the numerator factors. */
    for (int pi = 0; pi < 4; ++pi) {
        int p = primes[pi];
        int e = exps[pi];
        for (int k = 0; k < e; ++k) {
            bool cancelled = false;
            for (int i = 0; i < 10 && !cancelled; ++i) {
                if (nums[i] % p == 0) {
                    nums[i] /= p;
                    cancelled = true;
                }
            }
            /* If for some reason no factor could be cancelled the
               algorithm still works because the product would remain
               divisible by the remaining power of p. */
        }
    }

    /* Compute the product of the remaining numerator factors. */
    seq_t result = (seq_t)1;
    for (int i = 0; i < 10; ++i) {
        result *= nums[i];
    }

    return result;
}
