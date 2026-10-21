/*
 * A055983  a(n+1) = a(n) converted to base 10 from base 12.
 * https://oeis.org/A055983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(0) = 10; a(n+1) = value obtained by interpreting decimal
 *            digits of a(n) as a base‑12 number (i.e. multiply each decimal
 *            digit by the appropriate power of 12)
 * Range:     u8 n <= 17, u16 n <= 29, u32 n <= 39, u64 n <= 48, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cur = 10;                 /* a(0) */
    for (idx_t i = 0; i < n; ++i)   /* compute a(1) … a(n) */
    {
        seq_t next  = 0;
        seq_t power = 1;
        seq_t val   = cur;

        /* decompose cur into decimal digits and evaluate in base 12 */
        while (val != 0)
        {
            seq_t digit = val % 10;
            next += digit * power;
            power *= 12;
            val   /= 10;
        }
        cur = next;
    }
    return cur;
}
