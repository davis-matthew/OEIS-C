/*
 * A037532  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,1,2.
 * https://oeis.org/A037532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 1, * a(n) = a(n-1) * 7 + ((n % 3 == 0) ? 2 : 1), for n >=
 *            2 * (Base‑7 digits are the periodic sequence 1,1,2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)      /* offset is 1; behavior for n<1 is irrelevant */
        return 0;

    /* a(1) = 1 */
    if (n == 1u)
        return 1;

    /* Compute iteratively: a(n) = a(n-1) * 7 + next_digit */
    seq_t result = 1;           /* start with a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t digit = (i % 3 == 0) ? (seq_t)2 : (seq_t)1;
        result = result * (seq_t)7 + digit;
    }
    return result;
}
