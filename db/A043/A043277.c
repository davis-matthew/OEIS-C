/*
 * A043277  Maximal run length in base 3 representation of n.
 * https://oeis.org/A043277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of the longest run of identical digits in the
 *            base‑3 representation of n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle n = 0 even though the sequence is defined for n >= 1 */
    if (n == 0) {
        return (seq_t)0;
    }

    seq_t max_run = 1;   /* at least one digit exists */
    seq_t cur_run = 0;
    int last_digit = -1; /* no previous digit yet */

    while (n > 0) {
        int digit = (int)(n % 3);
        if (digit == last_digit) {
            cur_run++;
        } else {
            last_digit = digit;
            cur_run = 1;
        }
        if (cur_run > max_run) {
            max_run = cur_run;
        }
        n /= 3;
    }
    return max_run;
}
