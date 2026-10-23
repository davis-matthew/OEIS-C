/*
 * A021278  Decimal expansion of 1/274.
 * https://oeis.org/A021278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^{n+1}/274) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the (n+1)-st decimal digit of 1/274 via long division. */
seq_t a(idx_t n)
{
    const idx_t denom = 274ULL;
    idx_t r = 1;          /* current remainder */
    seq_t digit = 0;      /* last digit produced */

    for (idx_t i = 0; i <= n; ++i)
    {
        idx_t tmp = r * 10ULL;
        digit = tmp / denom;
        r = tmp % denom;
    }
    return digit;
}
