/*
 * A180853  Trajectory of 4 under map n->A006368(n).
 * https://oeis.org/A180853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [4,6,9,7,5][n mod 5] (period 5)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 5;
    switch (r)
    {
        case 0: return (seq_t)4;
        case 1: return (seq_t)6;
        case 2: return (seq_t)9;
        case 3: return (seq_t)7;
        default: return (seq_t)5; /* r == 4 */
    }
}
