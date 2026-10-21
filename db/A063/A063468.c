/*
 * A063468  Number of Pythagorean triples in the range [1..n], i.e., the
 *          number of integer solutions to x^2 + y^2 = z^2 with 1 <= x,y,z <=
 *          n.
 * https://oeis.org/A063468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{(a,b,c) primitive Pythagorean triple} 2·⌊n/c⌋ where
 *            the triple satisfies a²+b²=c², a,b,c>0, gcd(a,b)=1, a≠b, c≤n.
 *            The sum runs over all primitive triples; each multiple k·(a,b,c)
 *            is counted as 2 triples (ordered (x,y) and (y,x))
 * Range:     u8 n <= 201, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5)
        return 0;              /* no triples with c<5 */

    seq_t total = 0;

    /* Euclid's parameterisation: a=m²−n² , b=2mn , c=m²+n²  (m>n) */
    for (uint32_t m = 2; (uint64_t)m * m <= n; ++m)        /* m²≤n ⇒ m≤√n */
    {
        for (uint32_t k = 1; k < m; ++k)          /* k is the inner n  */
        {
            /* m and k must have opposite parity and be coprime */
            if (((m ^ k) & 1u) == 0u)
                continue;
            /* gcd(m,k)==1 */
            uint32_t a_val = m, b_val = k;
            while (b_val) { uint32_t t = b_val; b_val = a_val % b_val; a_val = t; }
            if (a_val != 1u)
                continue;

            uint32_t c = m * m + k * k;
            if (c > n)
                break;           /* further k will give larger c */

            seq_t cnt = 2 * ((seq_t)n / c);
            total += cnt;
        }
    }
    return total;
}
