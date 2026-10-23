/*
 * A011570  Stirling numbers of second kind S2(21,n).
 * https://oeis.org/A011570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = Stirling numbers of the second kind S(21,n)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n > 21 the value is 0.  For n ≤ 21 we compute it via the recurrence
   S(k,n) = n*S(k-1,n) + S(k-1,n-1) with S(0,0)=1. */

#include <stdbool.h>

static seq_t cache[22];
static bool ready = false;

static void compute_cache(void)
{
    /* previous row and current row */
    seq_t prev[22] = {0}, cur[22] = {0};

    /* S(0,0) = 1 */
    prev[0] = 1;

    for (idx_t k = 1; k <= 21; ++k) {
        /* cur[0] stays 0 for k > 0 */
        cur[0] = 0;
        for (idx_t n = 1; n <= k; ++n) {
            /* cur[n] = n*prev[n] + prev[n-1] */
            seq_t term1 = (seq_t)n * prev[n];
            seq_t term2 = prev[n-1];
            cur[n] = term1 + term2;
        }
        /* copy cur into prev for next iteration */
        for (idx_t n = 0; n <= k; ++n) {
            prev[n] = cur[n];
        }
    }

    /* store the final row S(21,n) into cache */
    for (idx_t n = 0; n <= 21; ++n) {
        cache[n] = prev[n];
    }
    ready = true;
}

seq_t a(idx_t n)
{
    if (!ready)
        compute_cache();

    if (n <= 21)
        return cache[n];
    /* n > 21 -> Stirling number is zero */
    return (seq_t)0;
}
