/*
 * A005151  Summarize the previous term (digits in increasing order), starting
 *          with a(1) = 1.
 * https://oeis.org/A005151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n+1) = A047842(a(n)), a(1) = 1 (describe the digit counts of
 *            the previous term, digits in increasing order)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t current = 1;
    if (n <= 1) return current;

    for (idx_t i = 2; i <= n; i++) {
        int digit_counts[10] = {0};
        seq_t temp = current;

        while (temp > 0) {
            int d = (int)(temp % 10);
            digit_counts[d]++;
            temp /= 10;
        }

        int new_digits[100];
        int digit_pos = 0;

        for (int d = 0; d < 10; d++) {
            if (digit_counts[d] > 0) {
                int count = digit_counts[d];
                if (count >= 10) {
                    new_digits[digit_pos++] = count / 10;
                    new_digits[digit_pos++] = count % 10;
                } else {
                    new_digits[digit_pos++] = count;
                }
                new_digits[digit_pos++] = d;
            }
        }

        seq_t next = 0;
        for (int j = 0; j < digit_pos; j++) {
            next = next * 10 + new_digits[j];
        }

        if (next == current) break;
        current = next;
    }

    return current;
}
