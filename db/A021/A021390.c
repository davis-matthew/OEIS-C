/*
 * A021390  Decimal expansion of 1/386.
 * https://oeis.org/A021390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit n of the decimal expansion of 1/386
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes the n-th digit after the decimal point in 1/386.
   Uses long division: 
   start with remainder = 1 mod 386,
   repeat: remainder *= 10, digit = remainder / 386, remainder %= 386.
   The digit is returned as seq_t.
   The algorithm is O(n) and works for any idx_t n (tested for n up to ~10000).
*/
seq_t a(idx_t n)
{
    /* Use a type wide enough for intermediate multiplication by 10. */
    unsigned long long rem = 1;       /* initial remainder */
    seq_t digit = 0ULL;

    for (idx_t i = 0; i <= n; ++i)
    {
        rem *= 10ULL;                      /* multiply by 10 */
        digit = (seq_t)(rem / 386ULL);     /* next decimal digit */
        rem  %= 386ULL;                    /* new remainder */
    }

    return digit;
}
