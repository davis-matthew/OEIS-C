/*
 * A021372  Decimal expansion of 1/368.
 * https://oeis.org/A021372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = digit of 1/368 at index n (0‑based)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to a(n) = ⌊(10 * (10^n mod 368)) / 368⌋. */
seq_t a(idx_t n)
{
    idx_t r = 1;  /* 10^0 mod 368 */
    idx_t i;

    for (i = 0; i < n; ++i)
    {
        r = (r * 10) % 368;   /* r = 10^(i+1) mod 368   */
    }

    /* r = 10^n mod 368; next digital digit */
    return (seq_t)((r * 10) / 368);
}
