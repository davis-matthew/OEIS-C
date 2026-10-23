/*
 * A024469  a(n) = s(1)s(n) + s(2)s(n-1) + ... + s(k)s(n+1-k), where k = [
 *          (n+1)/2 ], s = (Lucas numbers).
 * https://oeis.org/A024469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊(n+1)/2⌋} L_i · L_{n+1-i}, * where L_i are
 *            Lucas numbers (L_0 = 2, L_1 = 1, L_{k} = L_{k-1}+L_{k-2})
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute the n‑th term of A024469 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, but guard for 0 */

    /* Allocate array for Lucas numbers up to index n.  Size n+1 is needed. */
    size_t sz = (size_t)n + 1;
    seq_t *lucas = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!lucas) abort();

    /* Base cases: L_0 = 2, L_1 = 1.  For n == 1 we only need L_0 and L_1. */
    lucas[0] = (seq_t)2;
    if (n >= 1)
        lucas[1] = (seq_t)1;

    /* Compute remaining Lucas numbers up to L_n. */
    for (idx_t i = 2; i <= n; ++i) {
        lucas[i] = lucas[i - 1] + lucas[i - 2];
    }

    /* Sum the products as per the definition. */
    idx_t k = (n + 1) / 2;          /* floor((n+1)/2) */
    seq_t sum = (seq_t)0;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t left  = lucas[i];
        seq_t right = lucas[n + 1 - i];
        sum += left * right;
    }

    free(lucas);
    return sum;
}
