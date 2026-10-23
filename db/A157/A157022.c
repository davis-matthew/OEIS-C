/*
 * A157022  Expansion of (1+x+x^2+x^7+x^8+x^9)/(1+x^12).
 * https://oeis.org/A157022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = Σ_{k≥0} (-1)^k · [n−12k ∈ {0,1,2,7,8,9}]
 * Range:     i8 n <= 93, i16 n <= 93, i32 n <= 93, i64 n <= 93, i128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where [P] = 1 if P holds, else 0. */

seq_t a(idx_t n)
{
    /* Base values for indices 0..11 */
    static const seq_t base[12] = {1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0};

    if (n < 12)          /* direct lookup for the first 12 terms */
        return base[(idx_t)n];

    /* For n ≥ 12, the value alternates sign every 12 steps. */
    idx_t r   = n % 12;                /* remainder modulo 12 */
    seq_t val = base[r];               /* base value for this remainder */

    /* If the number of 12‑step blocks is odd, flip sign. */
    if ((n / 12) & 1u)
        val = -val;

    return val;
}
