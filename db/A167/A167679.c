/*
 * A167679  Replace odd digits with 2 and even digits with 1.
 * https://oeis.org/A167679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   Replace each decimal digit d of n with 1 if d is even, otherwise
 *            with 2. The result is the number whose decimal digits are the
 *            replacements
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* special case: a(0) = 1 */
    if (n == 0)
        return 1;

    seq_t result = 0;     /* accumulated output */
    seq_t mult    = 1;    /* 10^position of current digit */

    idx_t m = n;
    while (m > 0) {
        idx_t digit = m % 10;          /* current digit of n */
        seq_t repl  = (digit & 1) ? 2 : 1;  /* replacement digit */
        result += repl * mult;        /* append digit */
        mult   *= 10;                 /* move to next decimal place */
        m      /= 10;
    }
    return result;
}
