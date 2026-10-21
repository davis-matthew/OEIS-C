/*
 * A203685  Vandermonde sequence using x^2 + xy + y^2 applied to
 *          (1,2,6,...,n!).
 * https://oeis.org/A203685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{1 ≤ i < j ≤ n} (i!² + i!·j! + j!²)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           (the sequence is defined by the product of the binary
 *            operation x² + xy + y² applied to the factorials
 *            1!, 2!, 3!, …, n!).  */
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

seq_t a(idx_t n)
{
    assert(n >= 1);
    if (n == 1) return (seq_t)1;   /* a(1) = 1 */

    /* Allocate space for factorials 1! … n! (index 1..n) */
    seq_t *fact = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!fact) return (seq_t)0;    /* out of memory – not expected in tests */

    fact[1] = (seq_t)1;            /* 1! = 1 */
    for (idx_t i = 2; i <= n; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;   /* factorial recurrence */
    }

    seq_t prod = (seq_t)1;
    for (idx_t i = 1; i < n; ++i) {
        seq_t ai = fact[i];
        for (idx_t j = i + 1; j <= n; ++j) {
            seq_t bj = fact[j];
            seq_t term = ai * ai + ai * bj + bj * bj; /* i!² + i!·j! + j!² */
            prod *= term;
        }
    }

    free(fact);
    return prod;
}
