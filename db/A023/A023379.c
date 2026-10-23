/*
 * A023379  a(n+1) = a(n) converted to base 6 from base 5 (written in base
 *          10).
 * https://oeis.org/A023379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 5; for n ≥ 1, a(n+1) is obtained by writing a(n) in base
 *            5, then interpreting that digit string as a number in base 6
 *            (result written in base 10)
 * Range:     u8 n <= 18, u16 n <= 26, u32 n <= 32, u64 n <= 39, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)5;                /* a(1) */

    seq_t cur = (seq_t)5;               /* start value */
    for (idx_t i = (idx_t)1; i < n; ++i)
    {
        seq_t tmp   = cur;              /* copy for digit extraction */
        seq_t next = (seq_t)0;          /* result of conversion   */
        seq_t pow6 = (seq_t)1;          /* 6^i in sequence base   */

        /* Extract base‑5 digits and accumulate in base‑6 value */
        while (tmp > (seq_t)0)
        {
            seq_t digit = tmp % (seq_t)5;   /* digit in base 5       */
            next += digit * pow6;           /* add its contribution in base 6 */
            pow6 *= (seq_t)6;               /* advance to next position   */
            tmp   /= (seq_t)5;              /* shift right in base 5     */
        }
        cur = next;                    /* next term of the sequence  */
    }
    return cur;                        /* a(n) */
}
