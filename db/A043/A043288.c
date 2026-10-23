/*
 * A043288  Maximal run length in base-14 representation of n.
 * https://oeis.org/A043288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of the longest run of equal digits in the base‑14
 *            representation of n
 * Range:     u8 n <= 8494, u16 n <= 8494, u32 n <= 8494, u64 n <= 8494,
 *            u128 n <= 8494, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Numbers less than 14 consist of a single digit, so answer is 1. */
    if (n < 14) return (seq_t)1;

    idx_t tmp = n;
    int last_digit = -1;   /* sentinel value that cannot occur in base‑14 digits */
    int run = 0;
    int max_run = 1;

    while (tmp > 0) {
        int digit = (int)(tmp % 14); /* current least‑significant base‑14 digit */
        tmp /= 14;

        if (digit == last_digit) {
            run++;
        } else {
            run = 1;
            last_digit = digit;
        }

        if (run > max_run) max_run = run;
    }

    return (seq_t)max_run;
}
