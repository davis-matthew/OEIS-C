/*
 * A025800  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^11)).
 * https://oeis.org/A025800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of partitions of n into parts 2, 3, and 11.
   The number equals the number of non‑negative integer solutions
   of 2*x + 3*y + 11*z = n.  For each admissible z we count
   the solutions to 2*x + 3*y = n - 11*z, which amounts to
   summing over all y with the correct parity.  */
seq_t a(idx_t n)
{
    seq_t result = 0;
    /* maximum value of z such that 11*z <= n */
    idx_t max_z = n / 11;
    for (idx_t z = 0; z <= max_z; ++z) {
        idx_t m = n - 11 * z;             /* remaining amount after choosing z */
        idx_t max_y = m / 3;              /* largest y that can be used */
        idx_t first_y = (m & 1) ? 1 : 0;  /* y must have same parity as m */
        if (first_y <= max_y) {
            /* number of suitable y values = floor((max_y - first_y)/2) + 1 */
            seq_t add = (seq_t)((max_y - first_y) / 2 + 1);
            result += add;
        }
    }
    return result;
}
