/*
 * A162256  a(n) = (2*n^3 + 5*n^2 - 3*n)/2.
 * https://oeis.org/A162256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 + 5*n^2 - 3*n)/2
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* value before the offset, not used */

    /* Compute in a wider type to avoid overflow for 64‑bit searches. */
    unsigned __int128 t = n;
    unsigned __int128 val = (2 * t * t * t + 5 * t * t - 3 * t) / 2;

    return (seq_t)val;
}
