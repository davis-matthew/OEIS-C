/*
 * A138190  Numerator of (n-1)*n*(n+1)/12.
 * https://oeis.org/A138190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of (n‑1) * n * (n+1) / 12 i.e. a(n) =
 *            (n-1)*n*(n+1) / gcd((n-1)*n*(n+1), 12)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the product (n-1)*n*(n+1) in seq_t */
    seq_t nm1 = (seq_t)n - (seq_t)1;
    seq_t np1 = (seq_t)n + (seq_t)1;
    seq_t prod = nm1 * (seq_t)n * np1;          /* (n-1)·n·(n+1) */

    /* Compute gcd(prod, 12) using a small Euclidean loop.
       12 is cast to seq_t so the modulus works for all seq_t types. */
    seq_t g   = (seq_t)12;
    seq_t r   = prod % g;
    while (r != 0) {
        g = r;
        r = prod % g;
    }

    /* Divide by the gcd to obtain the numerator of the reduced fraction */
    return prod / g;
}
