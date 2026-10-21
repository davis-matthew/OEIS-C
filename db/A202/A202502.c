/*
 * A202502  Modified lower triangular Fibonacci matrix, by antidiagonals.
 * https://oeis.org/A202502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   The sequence enumerates the entries of the lower triangular
 *            Fibonacci matrix P (P[i][j] = F_{i-j+2} for i >= j, else 0) by
 *            antidiagonals. For a given n, let (i,j) be the position on the
 *            antidiagonal that contains the n‑th entry. Then a(n) = 0 if i <
 *            j a(n) = F_{i-j+2} otherwise where F_k is the k...
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Find the diagonal index m such that the first m diagonals contain
       at least n entries.  The first m diagonals contain m*(m+1)/2 entries.
       The loop below accumulates terms until the limit is reached. */
    idx_t m = 1;
    idx_t total = 0;
    while (1) {
        total += m;          /* total = m*(m+1)/2 after the loop */
        if (total >= n) break;
        ++m;
    }

    /* Total entries before the current diagonal. */
    idx_t prev_total = total - m;
    /* Zero–based offset within the current diagonal. */
    idx_t offset = n - prev_total - 1;

    /* Indices on the current diagonal (i+j = m+1). */
    idx_t i = offset + 1;
    idx_t j = m + 1 - i;

    /* Entries above the diagonal are zero. */
    if (i < j) return (seq_t)0;

    /* Difference i-j is non‑negative here.  Return Fibonacci(i-j+2). */
    idx_t diff = i - j;            /* diff >= 0 */
    return fib(diff + 2);
}
