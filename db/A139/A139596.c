/*
 * A139596  A033587(n) followed by even hexagonal number A014635(n+1).
 * https://oeis.org/A139596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*floor(n/2)^2 + 6*floor(n/2) if n is even * a(n) =
 *            8*ceil(n/2)^2 - 2*ceil(n/2) if n is odd
 * Range:     u8 n <= 10, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n >> 1;               /* floor(n/2)                                  */
    if ((n & 1) == 0) {             /* even n: first entry of row r                */
        seq_t rr = (seq_t)r;
        return 8 * rr * rr + 6 * rr;
    } else {                        /* odd n: second entry of row r                */
        seq_t rr = (seq_t)(r + 1);
        return 8 * rr * rr - 2 * rr;
    }
}
