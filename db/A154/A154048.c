/*
 * A154048  Number of planar triangular n X n X n nonnegative integer grids
 *          symmetric both under 120 degree rotation and reflection with every
 *          similarly oriented 4 X 4 X 4 subtriangle summing to 5.
 * https://oeis.org/A154048
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   For n < 10, a(n) = [2,6,3,5,1,6,1] respectively for n = 4..10.
 *            For n ≥ 10, the values repeat with period 4: a(10)=1, a(11)=4,
 *            a(12)=1, a(13)=2, a(14)=1, ... i.e. a(n) = pattern[(n-10) mod 4]
 *            where pattern = {1,4,1,2}
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) return 0;          /* offset not required */
    if (n < 10) {
        switch (n) {
            case 4: return (seq_t)2;
            case 5: return (seq_t)6;
            case 6: return (seq_t)3;
            case 7: return (seq_t)5;
            case 8: return (seq_t)1;
            case 9: return (seq_t)6;
            case 10: return (seq_t)1;
        }
    }
    static const seq_t pattern[4] = {(seq_t)1, (seq_t)4, (seq_t)1, (seq_t)2};
    idx_t r = (n - 10ULL) & 3ULL;     /* mod 4 */
    return pattern[r];
}
