/*
 * A043715  Numbers whose base-12 representation has an odd number of runs.
 * https://oeis.org/A043715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer whose base-12 representation has an odd
 *            number of runs
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t x = 1;
    for (;; x++) {
        seq_t last = x % (seq_t)12;
        seq_t tmp   = x / (seq_t)12;
        int runs = 1;
        while (tmp > 0) {
            seq_t d = tmp % (seq_t)12;
            if (d != last) {
                runs++;
                last = d;
            }
            tmp /= (seq_t)12;
        }
        if ((runs & 1) != 0) {
            count++;
            if (count == n)
                return x;
        }
    }
}
