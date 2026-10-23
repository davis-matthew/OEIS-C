/*
 * A034586  Log_2 of dimension of an irreducible Z_2 graded H-module for
 *          Clifford algebra Cl_n.
 * https://oeis.org/A034586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/2) + (n%8==1 or 7 ? 1 : n%8==4 ? -1 : 0)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offsets for n % 8: 0 1 2 3 4 5 6 7 */
    static const int o[8] = {0, 1, 0, 0, -1, 0, 0, 1};

    seq_t half = (seq_t)(n >> 1);   /* floor(n/2) */
    int     mod = n & 7;            /* n % 8 */

    if (o[mod] > 0)
        half += (seq_t)o[mod];
    else if (o[mod] < 0)
        half -= (seq_t)(-o[mod]);

    return half;
}
