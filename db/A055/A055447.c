/*
 * A055447  Base 7 distribution of first digit of mantissa following Benford's
 *          Law, to minimize chi-squared statistic.
 * https://oeis.org/A055447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the Benford base‑7 digit that minimizes the chi–squared statistic
   after each term of the sequence is generated.  This implements the greedy
   construction described in OEIS A055447.  For each position n we choose the
   digit d ∈ {1,…,6} such that the sequence 1,…,n with a(n)=d has the smallest
   ∑_k (O_k – E_k)²/E_k, where O_k is the observed count of digit k and
   E_k = n·p(k) with p(k) = log₇(k+1) – log₇(k).  The digit set is very small
   so the search is O(6n), which meets the timing requirement for n up to
   10000. */
#include <math.h>
#include <stdint.h>

static const long double log7 = logl(7.0L);
static const long double p[6] = {
    logl(2.0L) / log7,                         /* digit 1 */
    (logl(3.0L) - logl(2.0L)) / log7,          /* digit 2 */
    (logl(4.0L) - logl(3.0L)) / log7,          /* digit 3 */
    (logl(5.0L) - logl(4.0L)) / log7,          /* digit 4 */
    (logl(6.0L) - logl(5.0L)) / log7,          /* digit 5 */
    (logl(7.0L) - logl(6.0L)) / log7           /* digit 6 */
};

seq_t a(idx_t n)
{
    int count[6] = {0};    /* observed counts of digits 1..6 */
    int best_d = 0;        /* digit chosen for position n */
    idx_t i;

    for (i = 1; i <= n; ++i) {
        idx_t len = i;     /* length after adding the new digit */
        long double best_chi = 1e308L;  /* large number */

        /* try each candidate digit d */
        for (int d = 0; d < 6; ++d) {
            long double chi = 0.0L;
            for (int k = 0; k < 6; ++k) {
                long double cnt = count[k] + ((k == d) ? 1.0L : 0.0L);
                long double exp = (long double)len * p[k];
                long double diff = cnt - exp;
                chi += diff * diff / exp;
            }
            if (chi < best_chi) {
                best_chi = chi;
                best_d = d;
            }
        }

        /* accept the chosen digit */
        count[best_d] += 1;
        if (i == n) {
            return (seq_t)(best_d + 1);  /* digits are 1..6 */
        }
    }
    /* should never reach here */
    return (seq_t)0;
}
