/*
 * A044930  a(n)=so-se, where so(se)=sum of odd(even) base 8 run lengths of n.
 * https://oeis.org/A044930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = sum of odd‑run‑lengths minus sum of even‑run‑lengths * in
 *            the base‑8 representation of n (a(1) is the first term)
 * Range:     i8 n <= 83, i16 n <= 83, i32 n <= 83, i64 n <= 83, i128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is at least 1 (offset 1).  Convert to base 8 and process runs. */
    idx_t  temp = n;          /* remaining digits of n in base 8 */
    int    prev_digit = -1;   /* digit of the current run */
    int    run_len    = 0;    /* length of the current run */
    seq_t  diff       = 0;    /* result = sum_odd - sum_even */

    while (temp > 0) {
        int digit = (int)(temp & 7ULL);   /* temp % 8 */
        temp >>= 3;                       /* temp /= 8 */

        if (digit == prev_digit) {
            run_len++;                    /* extend current run */
        } else {
            if (prev_digit != -1) {        /* finish previous run */
                if ((run_len & 1) == 0) { /* even run length */
                    diff -= (seq_t)run_len;
                } else {                   /* odd run length */
                    diff += (seq_t)run_len;
                }
            }
            prev_digit = digit;           /* start new run */
            run_len = 1;
        }
    }

    /* finish the last run if any */
    if (prev_digit != -1) {
        if ((run_len & 1) == 0) {
            diff -= (seq_t)run_len;
        } else {
            diff += (seq_t)run_len;
        }
    }

    return diff;
}
