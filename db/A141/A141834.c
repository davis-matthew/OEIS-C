/*
 * A141834  Sum of the lengths of the cycles at a vertex of the complete graph
 *          K_n.
 * https://oeis.org/A141834
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2 - 2*n + (n-1)*(n-1)! * sum_{k=0}^{n-1} 1/k!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 3 expected */
    // compute sum_{k=0}^{n-1} (n-1)! / k! efficiently
    seq_t term = 1;            /* term == (n-1)! / fact[k] for current k */
    seq_t sum  = 0;
    idx_t k = n - 1;

    while (1) {
        sum += term;
        if (k == 0) break;
        term *= (seq_t)k;
        --k;
    }

    seq_t res = (seq_t)(n - 1) * sum + 2;
    res -= (seq_t)(2 * n);
    return res;
}
