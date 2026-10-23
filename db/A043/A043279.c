/*
 * A043279  Maximal run length in base 5 representation of n.
 * https://oeis.org/A043279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of the longest run of equal digits in the base-5
 *            representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n;
    seq_t max_run = 1;
    seq_t cur_run = 1;
    idx_t prev_digit = m % 5;
    m /= 5;

    while (m > 0) {
        idx_t digit = m % 5;
        if (digit == prev_digit) {
            cur_run += 1;
        } else {
            if (cur_run > max_run) max_run = cur_run;
            cur_run = 1;
            prev_digit = digit;
        }
        m /= 5;
    }

    if (cur_run > max_run) max_run = cur_run;
    return max_run;
}
