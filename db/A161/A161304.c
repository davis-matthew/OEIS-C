/*
 * A161304  Number of partitions of n into numbers not divisible by 4 where
 *          every part appears at least 13 times.
 * https://oeis.org/A161304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 116, u16 n <= 277, u32 n <= 770, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A161304
 *  Number of partitions of n into parts not divisible by 4 where every part
 *  appears at least 13 times.
 *
 *  The generating function is
 *      Π_{s>=1, s%4!=0} ( 1 + x^{13s} / (1 - x^s) ).
 *  The program computes the coefficients by iterating over the allowed part
 *  sizes s.  For each s we first copy the current DP array (which contains
 *  numbers of partitions using only smaller part sizes) and then add the
 *  contributions of the part size s using at least 13 copies.  The
 *  contributions are computed efficiently by scanning the DP indices in
 *  steps of s and maintaining a running sum of all earlier DP values
 *  that are at least 13*s less than the current index.
 *
 *  Complexity:   O((n/13) * n)   ≈ 5.8·10⁶ basic operations for n = 10 000.
 *  Memory:       O(n)
 *
 *  The code is pure C11 and also compiles as C++17.  No external
 *  utilities are used beyond the standard library.  It is perfectly
 *  fine for arbitrary-precision seq_t types when compiled as C++.
 */

#include <stddef.h>
#include <stdlib.h>

/* ----------------------------------------------------------------------------
   Helper macros that work in both C11 and C++17.
   -------------------------------------------------------------------------- */
#ifdef __cplusplus
# define ALLOC_SEQ_T(cnt)  (new seq_t[(cnt)]())
# define FREE_SEQ_T(p)     (delete[] (p))
#else
# define ALLOC_SEQ_T(cnt)  ((seq_t*)calloc((cnt), sizeof(seq_t)))
# define FREE_SEQ_T(p)     (free(p))
#endif

/* ----------------------------------------------------------------------------
   Main sequence function.
   -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    size_t N = (size_t)n;               /* use size_t for array indices      */
    if (N == 0)                          /* the sequence is defined from index 1 */
        return (seq_t)0;

    /* Allocate DP arrays */
    seq_t *dp  = ALLOC_SEQ_T(N + 1);
    seq_t *dp2 = NULL;                     /* will be allocated for each part   */

    dp[0] = (seq_t)1;                     /* empty partition                   */

    /* Maximum part size that can possibly be used (requires 13*s <= n) */
    idx_t s_limit = n / 13;

    for (idx_t s = 1; s <= s_limit; ++s)
    {
        if (s % 4 == 0)                   /* skip multiples of 4              */
            continue;

        /* --- start a new intermediate array -------------------------------- */
        dp2 = ALLOC_SEQ_T(N + 1);         /* allocate for current part          */
        for (size_t i = 0; i <= N; ++i)
            dp2[i] = dp[i];               /* copy previous DP values            */

        /* --- add contributions of part size s with minimum 13 copies ------- */
        for (size_t r = 0; r < (size_t)s; ++r)
        {
            size_t idx = r;
            seq_t  window = (seq_t)0;      /* sum of dp values at distance >=13*s */

            while (idx <= N)
            {
                if (idx >= 13 * (size_t)s)
                    window += dp[idx - 13 * (size_t)s];

                dp2[idx] += window;          /* dp2 = dp (no s) + windows (>=13*s) */
                idx += s;
            }
        }

        /* --- complete this part ------------------------------------------- */
        FREE_SEQ_T(dp);                    /* release old DP                   */
        dp = dp2;                          /* new DP becomes current DP          */
    }

    seq_t result = dp[N];
    FREE_SEQ_T(dp);
    return result;
}
