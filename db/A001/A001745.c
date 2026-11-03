/*
 * A001745  Numbers such that at least one digit contains a loop (version 2).
 *          Also called "holey" or "holy" numbers.
 * https://oeis.org/A001745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th number with at least one holey digit (0, 4, 6,
 *            8, 9)
 * Range:     u8 n <= 182, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 82 terms through n = 927 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static int has_holey_digit(seq_t num) {
    if (num == 0) return 1;
    while (num > 0) {
        int digit = (int)(num % 10);
        if (digit == 0 || digit == 4 || digit == 6 || digit == 8 || digit == 9) {
            return 1;
        }
        num /= 10;
    }
    return 0;
}

seq_t a(idx_t n) {
    seq_t target = (seq_t)n;
    seq_t count = 0;
    seq_t num = 0;
    while (1) {
        if (has_holey_digit(num)) {
            count++;
            if (count == target) {
                return num;
            }
        }
        num++;
    }
}
