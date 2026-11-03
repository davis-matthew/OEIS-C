/*
 * A007890  Summarize the previous term! (in decreasing order).
 * https://oeis.org/A007890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 1; a(n) = A244112(a(n-1)) for n > 1, i.e. describe the
 *            digits of a(n-1) in decreasing digit order (count followed by
 *            digit); a(n) = 14233221 for n >= 13
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 28 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t describe_decreasing(seq_t num) {
    int digit_count[10] = {0};
    seq_t temp = num;
    while (temp > 0) {
        int d = (int)(temp % 10);
        digit_count[d]++;
        temp /= 10;
    }
    seq_t result = 0;
    for (int d = 9; d >= 0; d--) {
        int c = digit_count[d];
        if (c > 0) {
            int digs[12];
            int k = 0;
            while (c > 0) {
                digs[k++] = c % 10;
                c /= 10;
            }
            while (k > 0) {
                k--;
                result = result * 10 + (seq_t)digs[k];
            }
            result = result * 10 + (seq_t)d;
        }
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t current = 1;
    for (idx_t i = 2; i <= n; i++) {
        seq_t next = describe_decreasing(current);
        if (next == current) break;
        current = next;
    }
    return current;
}
