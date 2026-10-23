/*
 * A023373  a(n+1) = a(n) converted to base 5 from base 4 (written in base
 *          10).
 * https://oeis.org/A023373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=4; for n≥1 a(n+1) = the number obtained by writing a(n) in
 *            base 4 and interpreting that digit string as a base‑5 number.
 *            Equivalently, a(n+1) = Σ (digit_i(a(n)) * 5^i), where digit_i is
 *            the i‑th digit of a(n) in base 4
 * Range:     u8 n <= 14, u16 n <= 20, u32 n <= 25, u64 n <= 29, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    seq_t value = 4;                 /* a(1) */
    if (n == 1) return value;

    for (idx_t i = 1; i < n; ++i)
    {
        seq_t next = 0;
        seq_t pow5 = 1;
        seq_t temp = value;

        /* Convert temp from base‑4 to base‑5 */
        while (temp != 0)
        {
            seq_t digit = temp % 4;   /* current base‑4 digit (0…3) */
            temp = temp / 4;          /* shift right by one base‑4 digit */
            next += digit * pow5;     /* add contribution of this digit */
            pow5 *= 5;                /* move to next higher base‑5 place */
        }

        value = next;
    }
    return value;
}
