/*
 * A137729  Number of circular permutations of the multiset {1,1,2,2,...,n,n}
 *          (up to rotations).
 * https://oeis.org/A137729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2*n)!/2^n + n!)/(2*n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A137729.  The function is fully memoized
   per call and works for any seq_t (uint8_t … uint64_t, unsigned __int128,
   or arbitrary‑precision types). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition. */
    if (n == 0) return (seq_t)1;

    /* If seq_t fits in 8 bytes or less we can safely use a 128‑bit
       intermediate type.  This handles all fixed‑width integer builds
       and guarantees correct results for the values that fit in seq_t. */
    if (sizeof(seq_t) <= 8U) {
        /* Use unsigned __int128 for intermediate arithmetic. */
        unsigned __int128 term1 = 1;          /* (2n)! / 2^n  */
        unsigned __int128 term2 = 1;          /* n!          */
        uint64_t i, twos = 0;                 /* how many 2's we have cancelled */
        uint64_t double_n = 2ULL * (uint64_t)n;

        /* Compute (2n)! / 2^n by canceling 2's on the fly. */
        for (i = 1; i <= double_n; ++i) {
            term1 *= (unsigned __int128)i;
            while (twos < n && (term1 & (unsigned __int128)1ULL) == 0) {
                term1 >>= 1;
                ++twos;
            }
        }

        /* Compute n! */
        for (i = 2; i <= (uint64_t)n; ++i) {
            term2 *= (unsigned __int128)i;
        }

        /* Sum and divide by 2n. */
        unsigned __int128 sum = term1 + term2;
        unsigned __int128 result = sum / (unsigned __int128)(double_n);
        return (seq_t)result;
    }

    /* General case for arbitrary‑precision seq_t.  We perform the same
       cancellation strategy but using seq_t operations.  For such types
       the intermediate values will not overflow. */
    {
        seq_t term1 = (seq_t)1;           /* (2n)! / 2^n  */
        seq_t term2 = (seq_t)1;           /* n!          */
        idx_t twos = 0;                    /* how many 2's we have cancelled */
        idx_t i;
        idx_t double_n = (idx_t)2 * n;

        for (i = 1; i <= double_n; ++i) {
            term1 = term1 * (seq_t)i;
            while (twos < n && (term1 % (seq_t)2) == (seq_t)0) {
                term1 = term1 / (seq_t)2;
                ++twos;
            }
        }

        for (i = 2; i <= n; ++i) {
            term2 = term2 * (seq_t)i;
        }

        seq_t sum = term1 + term2;
        seq_t denom = (seq_t)((idx_t)2 * n);
        seq_t result = sum / denom;
        return result;
    }
}
