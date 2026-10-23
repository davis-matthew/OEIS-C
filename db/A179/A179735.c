/*
 * A179735  Number of permutations of 1..n with the sequence of sums of 8
 *          adjacent elements having exactly 2 maxima.
 * https://oeis.org/A179735
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * C(n-7, 3) / 2^{n-8} (for n >= 9, a(9)=0)
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 11, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Terms below the offset are undefined – return 0 for safety */
    if (n < 9) return 0;

    /* ---------- cached factorials ----------
       We only need one array storing factorials up to the largest n
       seen so far.  This turns repeated calls (even in arbitrary order)
       into O(1) look‑ups after a linear pre‑computation. ---------- */
    static seq_t *fact = NULL;      /* factorial array */
    static idx_t  fact_len = 0;     /* highest n whose factorial is stored */

    /* extend the factorial cache if required */
    if (n > fact_len) {
        idx_t new_len = n + 1;                      /* include n itself   */
        /* allocate / reallocate the array */
        fact = (seq_t*)realloc(fact, new_len * sizeof(seq_t));
        /* initialise from previous state */
        if (fact_len == 0) {
            fact[0] = (seq_t)1;                    /* 0! = 1 */
            for (idx_t i = 1; i <= n; ++i)
                fact[i] = fact[i - 1] * (seq_t)i;   /* up to n!      */
        } else {
            for (idx_t i = fact_len + 1; i <= n; ++i)
                fact[i] = fact[i - 1] * (seq_t)i;   /* extend factorial */
        }
        fact_len = n;
    }

    /* ---------- compute the required binomial factor ----------
       C(n-7, 3) = (n-7)(n-8)(n-9) / 6  , valid for n >= 9   */
    seq_t comb = (seq_t)(n - 7) * (seq_t)(n - 8) * (seq_t)(n - 9) / 6;

    /* ---------- final formula ----------
       a(n) = n! * comb / 2^{n-8}  */
    seq_t numerator = fact[n] * comb;
    seq_t denominator = pow2(n - 8);      /* 2^{n-8}  */
    return numerator / denominator;
}
