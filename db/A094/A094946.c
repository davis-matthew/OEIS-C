/*
 * A094946  a(n) is 10110(3 1's)0...0(n 1's).
 * https://oeis.org/A094946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = a(n-1)*10^(n+2) + (10^(n+1)-1)/9, with a(0) = 1
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    seq_t value = (seq_t)1;      /* current a(k)  */
    seq_t shift = (seq_t)100;    /* 10^2, will become 10^(i+2) after each loop  */
    seq_t ones  = (seq_t)1;      /* will become (10^(i+1)-1)/9 after each loop */

    for (idx_t i = (idx_t)1; i <= n; ++i) {
        /* New multiplier and ones for this index */
        shift = shift * (seq_t)10;       /* shift = 10^(i+2) */
        ones  = ones * (seq_t)10 + (seq_t)1;  /* ones = (10^(i+1)-1)/9 */

        /* Update the sequence value */
        value = value * shift + ones;
    }

    return value;
}
