/*
 * A021162  Decimal expansion of 1/158.
 * https://oeis.org/A021162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor( (10^(n+1)) / 158 ) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This gives the n‑th digit after the decimal point of 1/158. */
seq_t a(idx_t n)
{
    seq_t rem = 1;      /* current remainder after each division */
    seq_t d   = 0;      /* digit produced in the current step */

    for (idx_t i = 0; i <= n; ++i)
    {
        /* multiply by 10 (fits in an unsigned int when rem < 158) */
        unsigned int tmp = (unsigned int)rem * 10;
        d   = tmp / 158; /* current digit */
        rem = tmp % 158; /* new remainder for next step */
    }
    return d;
}
