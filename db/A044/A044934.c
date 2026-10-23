/*
 * A044934  Number of runs of even length in the base 3 representation of n.
 * https://oeis.org/A044934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of runs of even length in the base‑3
 *            representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)               /* offset is 1, but for safety */
        return (seq_t)0;

    idx_t temp = n;
    int prev_digit = (int)(temp % 3);
    temp /= 3;
    int run_len = 1;
    int count   = 0;

    while (temp > 0) {
        int digit = (int)(temp % 3);
        if (digit == prev_digit) {
            run_len++;
        } else {
            if ((run_len & 1) == 0)   /* even length? */
                count++;
            run_len = 1;
            prev_digit = digit;
        }
        temp /= 3;
    }
    if ((run_len & 1) == 0) /* check the last run */
        count++;

    return (seq_t)count;
}
