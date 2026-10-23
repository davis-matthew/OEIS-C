/*
 * A110652  n^2 followed by n^3 followed by n^4 followed by n.
 * https://oeis.org/A110652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n/4) ^ e, where e = 1 if n ≡ 0 (mod 4), otherwise e
 *            = n mod 4 + 1
 * Range:     u8 n <= 14, u16 n <= 62, u32 n <= 1022, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 1‑based indexing;  n ≥ 1.  */
seq_t a(idx_t n)
{
    /* ceil(n/4) computed as integer division with rounding up */
    idx_t base = (n + 3) / 4;

    /* determine the exponent e */
    idx_t r = n % 4;
    idx_t exp = (r == 0) ? 1 : r + 1;

    /* compute base^exp (exp is 1..4, so simple loop is fine) */
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= (seq_t)base;

    return result;
}
