/*
 * A079475  Number described by n using the "Look and Say" rule.
 * https://oeis.org/A079475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, nice
 * Formula:   a(n) = number that is described by n interpreted as *
 *            look-and-say pairs; runs of two digits (count,digit) produce
 *            that * many copies of digit. A lone last digit describes itself.
 *            * Trailing zeros in the result are omitted
 * Range:     u8 n <= 32, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* build an array with the decimal digits of n in reverse order */
    idx_t t = n;
    int digits[22];          /* maximal 20 digits for a 64‑bit integer
                                plus a couple of spare positions   */
    int dlen = 0;
    do
    {
        digits[dlen++] = t % 10;
        t /= 10;
    } while (t > 0);

    seq_t result = 0;
    int i = dlen - 1;        /* start with most significant digit */
    while (i >= 0)
    {
        if (i > 0)           /* regular count–digit pair */
        {
            int cnt = digits[i];
            int val = digits[i - 1];
            for (int rep = 0; rep < cnt; ++rep)
                result = result * (seq_t)10 + (seq_t)val;
            i -= 2;
        }
        else                 /* leftover single digit */
        {
            int val = digits[0];
            result = result * (seq_t)10 + (seq_t)val;
            i -= 1;
        }
    }

    /* strip trailing zeros */
    while (result != 0 && result % (seq_t)10 == 0)
        result /= (seq_t)10;

    return result;
}
