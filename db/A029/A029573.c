/*
 * A029573  Number of permutations of an n-set containing a 6-cycle.
 * https://oeis.org/A029573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (1 - Σ_{k=0..⌊n/6⌋} (-1)^k/(k!*6^k))
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* No 6‑cycle possible for n < 6 */
    if (n < 6)
        return 0;

    idx_t maxk = n / 6;          /* floor(n/6) */

    /* Compute T₁ = n! / 6 by multiplying all numbers 1..n except 6. */
    seq_t term = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        if (i != 6)
            term *= (seq_t)i;
    }

    seq_t result = 0;

    /* a(n) = Σ_{k=1..maxk} (-1)^{k+1} Tₖ, where T_{k+1} = T_k / ((k+1)*6) */
    for (idx_t k = 1; k <= maxk; ++k)
    {
        if (k & 1)              /* k odd → sign + */
            result += term;
        else                    /* k even → sign - */
            result -= term;

        /* Prepare next term, if any */
        if (k < maxk)
            term /= ((seq_t)(k + 1) * 6);
    }

    return result;
}
