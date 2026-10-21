/*
 * A233036  The maximum number of I-tetrominoes that can be packed into an n X
 *          n array of squares when rotation is allowed.
 * https://oeis.org/A233036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0; a(2)=0; a(3)=0; a(4k)=4k^2; a(4k+2)=4k(k+1);
 *            a(2k+1)=k(k+1) for k≥2;
 * Range:     u8 n <= 31, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3)
        return 0;

    seq_t result = 0;
    idx_t mod = n & 3;   /* n % 4 */

    if (mod == 0) {
        seq_t nn = (seq_t)n;
        result = (nn * nn) / 4;
    } else if (mod == 2) {
        idx_t k = (n - 2) / 4;
        seq_t kk = (seq_t)k;
        result = 4 * kk * (kk + 1);
    } else { /* odd: n % 4 == 1 or 3, but n>=4 so n != 3 */
        idx_t k = (n - 1) / 2;
        seq_t kk = (seq_t)k;
        result = kk * (kk + 1);
    }

    return result;
}
