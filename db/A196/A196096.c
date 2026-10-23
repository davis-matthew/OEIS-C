/*
 * A196096  Occurrences of '11' in base 3 expansion of n.
 * https://oeis.org/A196096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of occurrences of the substring "11" in the base‑3
 *            representation of n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t count = 0;
    int prev_digit = -1;
    idx_t value = n;
    while (value > 0) {
        int digit = (int)(value % (idx_t)3);
        if (prev_digit == 1 && digit == 1) {
            count++;
        }
        prev_digit = digit;
        value /= (idx_t)3;
    }
    return count;
}
