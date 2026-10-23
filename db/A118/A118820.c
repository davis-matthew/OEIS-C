/*
 * A118820  Start with 1 and repeatedly place the first digit at the end of
 *          the number and add 2.
 * https://oeis.org/A118820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; for n ≥ 1 let x = a(n) and d be the
   first decimal digit of x.  Set
      y = x – d·10^(len(x)-1)
   then
      a(n+1) = y·10 + d + 2.
   (The sequence enters a 9‑term cycle after the first 6 terms.) */

seq_t a(idx_t n)
{
    if (n == 0)            /* safety for offset 0 */
        return (seq_t)0;

    seq_t val = (seq_t)1;   /* a(1) */
    if (n == 1)
        return val;

    for (idx_t i = 1; i < n; ++i)
    {
        seq_t temp  = val;
        seq_t power = (seq_t)1;

        /* find the first digit and the corresponding power of 10 */
        while (temp >= (seq_t)10)
        {
            temp  /= (seq_t)10;
            power *= (seq_t)10;
        }

        seq_t d = temp;
        seq_t y = val - d * power;
        val     = y * (seq_t)10 + d + (seq_t)2;
    }
    return val;
}
