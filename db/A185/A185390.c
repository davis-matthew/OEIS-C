/*
 * A185390  Triangular array read by rows. T(n,k) is the number of partial
 *          functions on n labeled objects in which the domain of definition
 *          contains exactly k elements such that for all i in {1,2,3,...},
 *          (f^i)(x) is defined.
 * https://oeis.org/A185390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where (r,c) are obtained by reading * the
 *            triangular array T(n,k) = binomial(n,k) * k^k * (n-k+1)^(n-k-1)
 *            * by rows. Special cases: T(0,0)=1 and T(n,n)=n^n
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 64, u64 n <= 151,
 *            u128 n <= 404, big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */
#include "../utils/big_pow.h"    /* seq_t big_pow(seq_t base, seq_t exp); */

/* Compute T(n,k) for 0 ≤ k ≤ n.  */
static seq_t compute_T(idx_t n, idx_t k)
{
    /* T(0,0) is defined to be 1. */
    if (n == 0 && k == 0)
        return (seq_t)1;

    /* T(n,n) = n^n. */
    if (k == n)
        return big_pow((seq_t)n, (seq_t)n);

    /* General case. */
    seq_t bin = binomial(n, k);

    /* k^k, with 0^0 = 1. */
    seq_t pk;
    if (k == 0)
        pk = (seq_t)1;
    else
        pk = big_pow((seq_t)k, (seq_t)k);

    /* (n-k+1)^(n-k-1), where n-k-1 ≥ 0 because k < n. */
    idx_t exp_n = n - k - 1;
    seq_t pow2 = big_pow((seq_t)(n - k + 1), (seq_t)exp_n);

    /* Multiply the three factors. */
    seq_t result = bin * pk;
    result = result * pow2;
    return result;
}

/* Main entry point: returns the nth term of A185390.  */
seq_t a(idx_t n)
{
    /* Determine row r and column c by counting rows until the
     * cumulative number of elements exceeds n.  */
    idx_t r = 0;       /* current row index */
    idx_t cumulative = 0;  /* number of elements before current row */

    /* Incrementally build the row counts: row r has (r+1) elements. */
    while (cumulative + (r + 1) <= n) {
        cumulative += (r + 1);
        r++;
    }

    /* Now r is the row containing position n and
     * cumulative = r*(r+1)/2, the number of elements before that row. */
    idx_t c = n - cumulative;   /* column within row r (0‑based) */

    return compute_T(r, c);
}
