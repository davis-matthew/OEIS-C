/*
 * A229432  Number of n X 2 0..6 arrays of the sum of the corresponding
 *          element, the element to the east and the element to the south in a
 *          larger (n+1) X 3 0..2 array.
 * https://oeis.org/A229432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(1) = 43 * a(2) = 1213 * a(3) = 31111 * a(4) = 775213 * a(n)
 *            = 37*a(n-1) - 342*a(n-2) + 936*a(n-3) - 1296*a(n-4) (n ≥ 5) *
 *            (OEIS A229432)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for fixed‑width types, if needed */

seq_t a(idx_t n)
{
    /* values outside the offset are undefined, but return 0 for safety */
    if (n < 1) return 0;

    /* return the first four explicit terms */
    if (n == 1) return (seq_t)43;
    if (n == 2) return (seq_t)1213;
    if (n == 3) return (seq_t)31111;
    if (n == 4) return (seq_t)775213;

    /* use a sliding window of the last four terms */
    seq_t a1 = (seq_t)43;
    seq_t a2 = (seq_t)1213;
    seq_t a3 = (seq_t)31111;
    seq_t a4 = (seq_t)775213;
    seq_t ai = 0;   /* current term */

    for (idx_t i = 5; i <= n; ++i) {
        ai = (seq_t)37  * a4
           - (seq_t)342 * a3
           + (seq_t)936  * a2
           - (seq_t)1296 * a1;

        /* shift the window */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = ai;
    }

    return ai;
}
