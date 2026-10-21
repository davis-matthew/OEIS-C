/*
 * A074930  Number of integers in {1, 2, ..., n!} that are coprime to n.
 * https://oeis.org/A074930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)! * φ(n)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A074930.
   a(n) = (n-1)! * phi(n), where phi is Euler's totient function. */
seq_t a(idx_t n)
{
    /* Compute (n-1)! */
    seq_t fact = 1;
    for (idx_t i = 2; i < n; ++i)
        fact *= (seq_t)i;

    /* Compute φ(n) by prime factorisation */
    seq_t phi = (seq_t)n;
    idx_t m = n;
    for (idx_t p = 2; p <= m / p; ++p)
    {
        if (m % p == 0)
        {
            while (m % p == 0)
                m /= p;
            phi -= phi / (seq_t)p;
        }
    }
    if (m > 1)
        phi -= phi / (seq_t)m;

    return fact * phi;
}
