/*
 * A141907  a(n) = Sum_{k=0..n-1} (Sum_{j=0..k+1} (-1)^j*binomial(n + 1, j)*(k
 *          + 1 - j)^n) mod 10 * 10^k.
 * https://oeis.org/A141907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{k=0}^{n-1} ( Sum_{j=0}^{k+1} (-1)^j * C(n+1,j) *
 *            (k+1-j)^n mod 10 ) * 10^k. This is equivalent to the decimal
 *            number whose k-th least‑significant digit is the Eulerian number
 *            A(n,k+1) modulo 10. We compute the digits by the well‑known
 *            recurrence for Eulerian numbers: A(r,k) = (k+1)*A(...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is indexed from 1 (offset 1).  For n==0 we return 0, though the
       sequence is not defined below the offset. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Use size_t for array indices; n fits comfortably in that type. */
    size_t N = (size_t)n;

    /* Allocate storage for the current row of Eulerian numbers (mod 10). */
    uint8_t *prev = (uint8_t *)malloc(N * sizeof(uint8_t));
    if (!prev) {
        return (seq_t)0;   /* allocation failure – return sentinel */
    }
    prev[0] = 1;                 /* row n = 1: only digit 1 */

    /* Build rows 2 … n using the recurrence. */
    for (size_t r = 2; r <= N; ++r) {
        uint8_t *cur = (uint8_t *)malloc(r * sizeof(uint8_t));
        if (!cur) {
            free(prev);
            return (seq_t)0;
        }
        cur[0] = 1;                   /* A(r,0) = 1 for all r ≥ 1 */

        for (size_t k = 1; k < r; ++k) {
            int term1 = 0;
            if (k < r - 1) {
                term1 = (int)(k + 1) * prev[k];   /* (k+1)*A(r-1,k) */
            }
            int term2 = (int)(r - k) * prev[k - 1]; /* (r-k)*A(r-1,k-1) */
            cur[k] = (uint8_t)((term1 + term2) % 10);
        }

        free(prev);
        prev = cur;
    }

    /* Assemble the number from the computed digits (least‑significant first). */
    seq_t result = (seq_t)0;
    for (size_t idx = N; idx > 0; --idx) {
        result = result * 10 + (seq_t)prev[idx - 1];
    }

    free(prev);
    return result;
}
