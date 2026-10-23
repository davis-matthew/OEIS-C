/*
 * A179680  The number of exponents >1 in a recursive reduction of 2n-1 until
 *          reaching an odd part equal to 1.
 * https://oeis.org/A179680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count of 2‑adic valuations v_i > 1 in the recursive
 *            reduction of 2n‑1 until the odd part becomes 1
 * Range:     u8 n <= 64, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t __v2(seq_t x)
{
    seq_t k = 0;
    while ((x & ((seq_t)1)) == ((seq_t)0))
    {
        x >>= 1;
        ++k;
    }
    return k;
}

static inline seq_t __odd_part(seq_t x)
{
    while ((x & ((seq_t)1)) == ((seq_t)0))
        x >>= 1;
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety; a(1) is required anyway */

    seq_t N = ((seq_t)2) * ((seq_t)n) - ((seq_t)1); /* N = 2n-1 */
    seq_t count = ((seq_t)0);

    /* first valuation v(1) from N+1 = 2n */
    seq_t v = __v2(N + ((seq_t)1));
    if (v > ((seq_t)1)) ++count;

    /* initial odd part m(1) */
    seq_t m = __odd_part(N + ((seq_t)1));

    /* subsequent steps until m becomes 1 */
    while (m != ((seq_t)1))
    {
        v = __v2(N + m);
        if (v > ((seq_t)1)) ++count;
        m = __odd_part(N + m);
    }

    return count;
}
