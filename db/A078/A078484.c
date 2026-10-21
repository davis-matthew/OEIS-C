/*
 * A078484  G.f.: -x*(1-2*x+2*x^2)/(2*x^3-4*x^2+4*x-1).
 * https://oeis.org/A078484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n‑1) - 4*a(n‑2) + 2*a(n‑3) with a(0)=0, a(1)=1,
 *            a(2)=2, a(3)=6
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  Store terms up to n = 10000 (inclusive).  */
    static seq_t cache[10001] = {0, 1, 2, 6};
    /*  Highest index that has already been computed.  */
    static idx_t computed = 3;

    /*  Return already‑computed value if possible.  */
    if (n <= computed)
        return cache[n];

    /*  Compute new terms up to n.  */
    for (idx_t i = computed + 1; i <= n; ++i)
        cache[i] = (seq_t)4 * cache[i - 1]
                 - (seq_t)4 * cache[i - 2]
                 + (seq_t)2 * cache[i - 3];

    computed = n;
    return cache[n];
}
