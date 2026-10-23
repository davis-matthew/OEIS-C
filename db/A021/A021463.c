/*
 * A021463  Decimal expansion of 1/459.
 * https://oeis.org/A021463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(0) = 0. For n ≥ 1, a(n) is the (n + 1)-th decimal digit of
 *            1/459, obtained by the standard long‑division recurrence: r₀ =
 *            1, r_k = (10 · r_{k‑1}) mod 459, a(n) = floor(10 · r_{n‑1} /
 *            459)
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t rem = 1;                     /* r₀ = 1 */
    for (idx_t i = 1; i <= n; ++i)     /* compute r_{n‑1} */
    {
        rem *= 10;
        rem %= 459;
    }

    /* The quotient of 10·rem divided by 459 is the next decimal digit. */
    return (rem * 10) / 459;
}
