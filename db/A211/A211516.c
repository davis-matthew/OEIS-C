/*
 * A211516  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w^2=x+y.
 * https://oeis.org/A211516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{w=1}^{n} f(w^2) where * f(S) = { S-1 if 2 ≤ S ≤ n *
 *            2n-S+1 if n < S ≤ 2n * 0 otherwise } * This counts ordered
 *            triples (w,x,y) with 1≤w,x,y≤n and w^2 = x+y
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    seq_t nS = (seq_t)n;
    seq_t twoN = nS + nS;
    for (idx_t w = 1; w <= n; ++w) {
        seq_t S = (seq_t)w * w;
        if (S <= nS)               // 2 ≤ S ≤ n
            total += S - 1;
        else if (S <= twoN)        // n < S ≤ 2n
            total += twoN - S + 1;
        else                       // S > 2n, no further solutions
            break;
    }
    return total;
}
