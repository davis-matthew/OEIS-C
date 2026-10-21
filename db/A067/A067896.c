/*
 * A067896  Trajectory of 41 under map x -> x/2 if x even, x-> 3x+3 if x odd.
 * https://oeis.org/A067896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 41; a(n+1) = (a(n)/2 if a(n) even else 3*a(n)+3)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cur = 41;          /* current value of the trajectory     */
    seq_t one   = 1;         /* constant for parity testing        */

    for (idx_t i = 0; i < n; ++i) {
        if ((cur & one) == 0)          /* a(n) even    */
            cur = cur / 2;
        else                           /* a(n) odd     */
            cur = cur * 3 + 3;
    }

    return cur;
}
