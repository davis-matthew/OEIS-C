/*
 * A011568  Stirling numbers of second kind S2(19,n).
 * https://oeis.org/A011568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = Stirling number of the second kind S(19,n)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>            /* for bool */

static seq_t S[20] = {0};       /* S[k] will hold S(19,k) after initialization */
static bool init = false;       /* flag so that the table is built only once */

seq_t a(idx_t n)
{
    /* Build the values S(19,k) on first call, using the recurrence
       S(i,k) = k*S(i-1,k) + S(i-1,k-1).  The table is filled in-place
       from the top row (i = 1) up to i = 19, and ultimately holds
       S(19,k) for k = 1…19.  The array indices 0 and 20 are unused.   */
    if (!init) {
        init = true;
        /* base case: S(1,1) = 1 */
        S[1] = (seq_t)1;
        /* compute rows 2..19 */
        for (idx_t row = 2; row <= 19; ++row) {
            for (idx_t k = row; k >= 1; --k) {
                S[k] = (seq_t)k * S[k] + S[k - 1];
            }
        }
    }

    if (n >= 1 && n <= 19)
        return S[(idx_t)n];
    /* Undefined indices: return 0 */
    return (seq_t)0;
}
