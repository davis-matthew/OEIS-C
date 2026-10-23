/*
 * A182026  a(n) = 288*binomial(2*n,n-5) + 8*Sum_{i=1..n-5}
 *          binomial(2*n,n-5-i)*(5+i).
 * https://oeis.org/A182026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 288*binomial(2*n,n-5) + 8*Sum_{i=1..n-5}
 *            binomial(2*n,n-5-i)*(5+i)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5)
        return 0;

    /* Work with 2*n as seq_t to avoid mixing types */
    seq_t two_n = (seq_t)2 * (seq_t)n;

    /* Sum_{j=0}^{n-6} binomial(2n,j)*(n-j) */
    seq_t sum  = 0;
    seq_t curr = 1;  /* binom(2n,0) */

    for (idx_t k = 0; k < n - 5; ++k) {
        /* Add term for j = k : binom(2n,k)*(n-k) */
        sum += curr * ((seq_t)n - (seq_t)k);

        /* Advance to binom(2n, k+1) */
        curr = curr * (two_n - (seq_t)k) / ((seq_t)k + 1);
    }

    /* At this point curr == binom(2n, n-5) */
    seq_t result = 288 * curr + 8 * sum;
    return result;
}
