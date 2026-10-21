/*
 * A095482  a(n) = 45 written in base n.
 * https://oeis.org/A095482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number obtained by writing the decimal number 45 in base
 *            n and interpreting the resulting digits as a decimal integer.
 *            For n = 1 the base‑1 representation consists of 45 ones (111…1)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle base 1 separately */
    if (n == (idx_t)1)
    {
        /* 45 ones → (10^45‑1)/9 as a decimal integer.  Build it without
           overflow for arbitrary‑precision seq_t; for 64‑bit seq_t the value
           will overflow but the test harness ignores it because it does not
           fit. */
        seq_t res = 0;
        for (idx_t i = 0; i < 45; ++i)
            res = res * ((seq_t)10) + ((seq_t)1);
        return res;
    }

    /* for n >= 2 compute the base‑n digits of 45 and then build the
       decimal number from those digits. */
    seq_t val = 45;
    /* maximum number of digits is 6 (binary 101101) so an array of 6 is safe */
    unsigned char digits[6];
    int d = 0;
    while (val > 0 && d < 6)
    {
        digits[d++] = (unsigned char)(val % n);
        val = val / n;
    }
    /* digits are stored least‑significant first; construct the decimal number */
    seq_t result = 0;
    for (int i = d - 1; i >= 0; --i)
        result = result * ((seq_t)10) + ((seq_t)digits[i]);

    return result;
}
