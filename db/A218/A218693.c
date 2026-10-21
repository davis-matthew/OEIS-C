/*
 * A218693  a(n) = Sum_{k=0..n} C(n,k)*C(n+k,k)^3.
 * https://oeis.org/A218693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k) * C(n+k,k)^3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) for OEIS A218693. */
seq_t a(idx_t n)
{
    /* Convert the index to seq_t for arithmetic. */
    seq_t n_seq = (seq_t)n;

    seq_t sum = 0;          /* The accumulating sum. */
    seq_t Cnk = 1;          /* Current binomial C(n,k).      */
    seq_t T_k  = 1;          /* Current binomial C(n+k,k).    */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* Add the term Cnk * T_k^3 to the sum. */
        seq_t t2 = T_k * T_k;        /* T_k^2 */
        seq_t term = Cnk * t2 * T_k; /* Cnk * T_k^3 */
        sum += term;

        if (k == n) break;           /* No more terms to update. */

        /* Update C(n,k+1) = C(n,k) * (n - k) / (k+1). */
        seq_t num = n_seq - (seq_t)k;
        Cnk = Cnk * num / (seq_t)(k + 1);

        /* Update T_{k+1} = T_k * (n + k + 1) / (k+1). */
        seq_t numk = n_seq + (seq_t)(k + 1);
        T_k = T_k * numk / (seq_t)(k + 1);
    }

    return sum;
}
