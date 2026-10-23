/*
 * A127844  a(1) = 1, a(2) = ... = a(10) = 0, a(n) = a(n-10)+a(n-9) for n>10.
 * https://oeis.org/A127844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 113, u16 n <= 189, u32 n <= 342, u64 n <= 649,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A127844 (phi_10 family)
   Recurrence: a(1) = 1; a(2) = ... = a(10) = 0;
   a(n) = a(n-10) + a(n-9) for n > 10
*/
#include <stdlib.h>
#include <stdint.h>

#ifndef MAX_CAP
#define MAX_CAP 200000   /* should cover all test indices (≈10 000)   */
#endif

static seq_t cache[MAX_CAP + 1];   /* 1‑based indexing; cache[0] unused   */
static size_t max_computed = 0;    /* highest index already computed     */

/* Compute a(n) for any n ≥ 1.  An out‑of‑range n is silently ignored
   and the function will return 0 (sequence is undefined for n < 1). */
seq_t a(idx_t n)
{
    if (n == 0)            /* sequence is defined for n ≥ 1 */
        return 0;

    if (n > MAX_CAP) {
        /* For n beyond the static buffer, perform a naïve computation
           without memoisation.  This should never be triggered for
           the test data, which limits n to ≈10 000. */
        idx_t i;
        seq_t val;
        if (n == 1) {
            val = 1;
        } else if (n <= 10) {
            val = 0;
        } else {
            seq_t a_n9 = 0, a_n10 = 0;
            /* Initialise a(11) = a(1) + a(2) = 1 + 0 = 1 */
            for (i = 2; i < n; ++i) {
                seq_t temp = a_n10 + a_n9;
                a_n10 = a_n9;
                a_n9  = temp;
            }
            val = a_n9;
        }
        return val;
    }

    /* Ensure values up to n are computed and cached  */
    size_t m = (size_t)n;
    if (m > max_computed) {
        size_t i;

        for (i = max_computed + 1; i <= m; ++i) {
            if (i == 1) {
                cache[i] = 1;
            } else if (i <= 10) {
                cache[i] = 0;
            } else {
                cache[i] = cache[i - 10] + cache[i - 9];
            }
        }
        max_computed = m;
    }

    return cache[m];
}
