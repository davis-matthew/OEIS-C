/*
 * A126246  a(n) is the number of Fibonacci numbers among
 *          (F(1),F(2),F(3),...,F(n)) which are coprime to F(n), where F(n) is
 *          the n-th Fibonacci number.
 * https://oeis.org/A126246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = φ(n) if n is odd, a(n) = 3*φ(n)/2 if n ≡ 0 mod 4, a(n) =
 *            2*φ(n) if n ≡ 2 mod 4 (n ≥ 1)
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes Euler's totient φ(n) for n ≥ 1.  n is an idx_t (uint64_t) and the
   result is returned as a seq_t.  The implementation performs a simple
   trial‑division factorisation; it requires only arithmetic operators and
   comparisons. */
static seq_t _phi_idx(idx_t n)
{
    if (n == 0u)
        return (seq_t)0;
    seq_t phi = (seq_t)n;
    idx_t t   = n;

    /* handle factor 2 */
    if ((t & 1u) == 0u) {
        phi -= phi / (seq_t)2;
        while ((t & 1u) == 0u)
            t >>= 1;
    }

    /* handle odd factors */
    for (idx_t p = 3u; p <= t / p; p += 2u) {
        if (t % p == 0u) {
            phi -= phi / (seq_t)p;
            while (t % p == 0u)
                t /= p;
        }
    }

    /* t is now a prime > 1, if it exists */
    if (t > 1u)
        phi -= phi / (seq_t)t;

    return phi;
}

seq_t a(idx_t n)
{
    if (n < 1u)
        return (seq_t)0;

    seq_t ph = _phi_idx(n);

    if ((n & 1u) != 0u)                 /* n is odd */
        return ph;
    else if ((n & 3u) == 0u)            /* n ≡ 0 (mod 4) */
        return (ph * (seq_t)3) / (seq_t)2;
    else                                /* n ≡ 2 (mod 4) */
        return ph * (seq_t)2;
}
