/*
 * A043280  Maximal run length in base 6 representation of n.
 * https://oeis.org/A043280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of the longest run of identical digits in the
 *            base‑6 representation of n. For n ≥ 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is an unsigned integer; 0 is not part of the sequence,
       but we handle it gracefully. */
    if (n == 0)
        return 0;

    int prevDigit = -1;  /* no previous digit yet */
    int curRun    = 0;   /* run length of current digit */
    int maxRun    = 1;   /* at least one digit in any positive number */

    idx_t value = n;
    while (value > 0) {
        int digit = (int)(value % 6);   /* base‑6 digit */
        if (digit == prevDigit) {
            curRun++;
        } else {
            prevDigit = digit;
            curRun = 1;
        }
        if (curRun > maxRun)
            maxRun = curRun;
        value /= 6;
    }
    return (seq_t)maxRun;
}
