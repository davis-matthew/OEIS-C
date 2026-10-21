/*
 * A078136  Numbers having exactly one representation as sum of squares>1.
 * https://oeis.org/A078136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A078136: numbers having exactly one representation as a sum of
   squares > 1.  The algorithm generates all sums of squares of integers
   ≥2 up to a fixed bound, counts unordered representations, and collects
   the numbers whose count equals one.  Since the sequence is finite
   (last term 39), a bound of 100 is more than sufficient. */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t terms[64];
    static size_t term_count = 0;
    enum { MAX_VAL = 100 };          /* safe upper bound for all terms */
    static seq_t dp[MAX_VAL + 1];

    if (!init) {
        /* initialize dp array */
        for (size_t i = 0; i <= (size_t)MAX_VAL; ++i)
            dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;

        /* list squares i^2 with i>=2 (fits in idx_t) */
        idx_t squares[64];
        size_t sqcnt = 0;
        for (idx_t i = 2; i * i <= (idx_t)MAX_VAL; ++i)
            squares[sqcnt++] = i * i;

        /* unbounded knapsack: count unordered sums */
        for (size_t si = 0; si < sqcnt; ++si) {
            size_t s = (size_t)squares[si];
            for (size_t j = s; j <= (size_t)MAX_VAL; ++j)
                dp[j] = dp[j] + dp[j - s];
        }

        /* collect numbers with exactly one representation */
        for (size_t val = 4; val <= (size_t)MAX_VAL; ++val) {
            if (dp[val] == (seq_t)1) {
                if (term_count < sizeof(terms)/sizeof(terms[0]))
                    terms[term_count++] = (seq_t)val;
            }
        }
        init = true;
    }

    if (n >= 1 && (idx_t)n <= term_count)
        return terms[(size_t)(n - 1)];
    return (seq_t)0;
}
