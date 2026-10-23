/*
 * A175870  Partial sums of ceiling(n^2/24).
 * https://oeis.org/A175870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((2*n^3 + 3*n^2 + 96*n)/144)
 * Range:     u8 n <= 2, u16 n <= 31, u32 n <= 900, u64 n <= 900,
 *            u128 n <= 900, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t to avoid intermediate overflow on 64‑bit builds */
    seq_t sn  = (seq_t)n;          /* n */
    seq_t sn2 = sn * sn;           /* n^2 */
    seq_t sn3 = sn2 * sn;          /* n^3 */

    /* Compute numerator = 2*n^3 + 3*n^2 + 96*n */
    seq_t num = 2 * sn3 + 3 * sn2 + 96 * sn;

    /* Ceiling division by 144: (num + 143) / 144 */
    return (num + 143) / 144;
}
