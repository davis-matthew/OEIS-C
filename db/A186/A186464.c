/*
 * A186464  Number of (n+1) X 2 0..2 arrays with every 2 X 2 subblock
 *          commuting with each of its horizontal and vertical 2 X 2 subblock
 *          neighbors.
 * https://oeis.org/A186464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence * a(n) = 2*a(n-1) + a(n-2) - 4*a(n-3) +
 *            4*a(n-4) - 2*a(n-5) for n > 10 * with initial values: * a(1)=81,
 *            a(2)=83, a(3)=277, a(4)=367, a(5)=966, * a(6)=1321, a(7)=3078,
 *            a(8)=4553, a(9)=10028, a(10)=15650 * The recurrence holds for
 *            all n ≥ 11. It uses only +,-,* on seq_t
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 30, u64 n <= 67, u128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base terms (index 1..10) */
    static const seq_t base[10] = {
        81, 83, 277, 367, 966,
        1321, 3078, 4553, 10028, 15650
    };

    if (n <= 10) {
        /* Safely indexed by n-1 (n >= 1) */
        return base[n-1];
    }

    /* Sliding window of the last 5 terms:
     * t0 = a(n-5), t1 = a(n-4), t2 = a(n-3), t3 = a(n-2), t4 = a(n-1) */
    seq_t t0 = base[5];   /* a(6)   */
    seq_t t1 = base[6];   /* a(7)   */
    seq_t t2 = base[7];   /* a(8)   */
    seq_t t3 = base[8];   /* a(9)   */
    seq_t t4 = base[9];   /* a(10)  */

    /* Compute iteratively up to n */
    for (idx_t i = 11; i <= n; ++i) {
        seq_t curr = 2*t4 + t3 - 4*t2 + 4*t1 - 2*t0;
        /* Shift window for next iteration */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = curr;
    }
    return t4;
}
