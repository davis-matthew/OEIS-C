/*
 * A198717  Number of n X 2 0..6 arrays with values 0..6 introduced in row
 *          major order and no element equal to any horizontal or vertical
 *          neighbor.
 * https://oeis.org/A198717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence (valid for n >= 6) a(n) = 55*a(n‑1) –
 *            918*a(n‑2) + 5818*a(n‑3) – 13417*a(n‑4) + 8463*a(n‑5) with
 *            initial values a(1)=1, a(2)=4, a(3)=34, a(4)=499, a(5)=10507
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial terms */
    const seq_t base[5] = { (seq_t)1, (seq_t)4, (seq_t)34,
                            (seq_t)499, (seq_t)10507 };

    if (n == 0) return (seq_t)0;       /* undefined for n=0, return 0 as placeholder */
    if (n <= 5) return base[n-1];

    /* Keep only the last five values */
    seq_t v0 = base[0], v1 = base[1], v2 = base[2], v3 = base[3], v4 = base[4];
    for (idx_t i = 6; i <= n; ++i) {
        seq_t new_val = 55*v4
                      - 918*v3
                      + 5818*v2
                      - 13417*v1
                      + 8463*v0;
        v0 = v1; v1 = v2; v2 = v3; v3 = v4; v4 = new_val;
    }
    return v4;
}
