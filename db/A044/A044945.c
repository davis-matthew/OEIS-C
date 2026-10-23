/*
 * A044945  Runs of odd length in the base 5 representation of n.
 * https://oeis.org/A044945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of runs of odd length in the base‑5 representation
 *            of n
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* offset starts at 1, n=0 is undefined */
    seq_t count = 0;
    idx_t tmp = n;
    int last_digit = -1;
    int run_len = 0;
    while (tmp > 0) {
        int digit = (int)(tmp % 5);
        if (digit == last_digit) {
            run_len++;
        } else {
            if (last_digit != -1 && (run_len & 1)) {
                count++;
            }
            last_digit = digit;
            run_len = 1;
        }
        tmp /= 5;
    }
    if (run_len & 1) {
        count++;
    }
    return count;
}
