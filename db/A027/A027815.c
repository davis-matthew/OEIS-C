/*
 * A027815  a(n) = 42*(n+1) * binomial(n+5,10).
 * https://oeis.org/A027815
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 42*(n+1) * binomial(n+5,10)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 20, u64 n <= 158, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute r = binomial(n+5,10) multiplicatively */
    seq_t r = 1;
    for (idx_t i = 1; i <= 10; ++i)
    {
        idx_t num = n + 5 - (10 - i);  /* numerator = n+5 - (10-i) */
        idx_t den = i;                /* denominator = i */
        /* multiply then divide using 128‑bit intermediate to avoid overflow */
        r = (seq_t)(((__int128)r * num) / den);
    }

    /* compute multiplier 42*(n+1) */
    seq_t mult = 42 * ((seq_t)n + 1);

    /* final result */
    return (seq_t)(((__int128)r * mult));
}
