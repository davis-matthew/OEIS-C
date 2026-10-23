/*
 * A115847  Integers i such that 17*i = 17 X i, i.e., 16*i XOR i = 17*i.
 * https://oeis.org/A115847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer i such that (i & (i>>4)) == 0
 * Range:     u8 n <= 80, u16 n <= 4095, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (seq_t i = 0;; ++i) {
        if ((i & (i >> 4)) == 0) {
            if (count == n)
                return i;
            ++count;
        }
    }
}
