/*
 * A154087  Number of planar triangular n X n X n nonnegative integer grids
 *          symmetric both under 120 degree rotation and reflection with every
 *          similarly oriented 5 X 5 X 5 subtriangle summing to 12.
 * https://oeis.org/A154087
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6 if n>=16 and n≡1 (mod 5), otherwise 9 for n>=16; For
 *            5≤n<16 the values are taken from a pre‑computed table
 * Range:     u8 n <= 51, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* values for n = 5 … 15 */
    if (n < 5)
        return (seq_t)0;          /* behavior below the offset is undefined */
    if (n < 16) {
        static const seq_t tbl[11] = {
            22, 40, 80, 53, 27, 67, 30, 17, 28, 9, 10   /* a(5)…a(15) */
        };
        return tbl[n - 5];
    }
    /* n >= 16 : periodic pattern 6,9,9,9,9 with period 5 */
    return (n % 5 == 1) ? (seq_t)6 : (seq_t)9;
}
