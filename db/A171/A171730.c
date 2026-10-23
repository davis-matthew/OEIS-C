/*
 * A171730  Triangle of differences of Fibonacci numbers, rows descending.
 * https://oeis.org/A171730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 276, u32 n <= 1081, u64 n <= 4278,
 *            u128 n <= 8372, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Triangle of differences of Fibonacci numbers, rows descending:
   a(1) = 1.
   For n ≥ 2, let r be the unique integer such that r(r‑1)/2 < n ≤ r(r+1)/2.
   Let pos = n – r(r‑1)/2 – 1 (0‑based index inside row r).
   Then k = 0 if pos == 0 else pos+1, and
   a(n) = F(r+1) – F(k).  */
/* This is an exact, O(r) implementation that handles any n ≥ 1 with
   64‑bit or arbitrary‑precision seq_t. */

seq_t a(idx_t n)
{
    /* Find the row number r by accumulating row lengths. */
    idx_t sum = 0;    /* number of terms before current row */
    idx_t row = 1;    /* current row length */
    while (sum + row < n) {
        sum += row;
        ++row;
    }

    /* pos is the 0‑based position within this row. */
    idx_t pos = n - sum - 1;

    /* Compute Fibonacci numbers up to F(row+1). */
    idx_t need = row + 2;                 /* indices 0 .. row+1 inclusive */
    seq_t *fib = (seq_t*)malloc(need * sizeof(seq_t));
    if (!fib) return 0;                  /* unsafe fallback – should never occur */

    fib[0] = 0;
    fib[1] = 1;
    for (idx_t i = 2; i <= row + 1; ++i) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }

    /* Determine the Fibonacci index k to subtract. */
    idx_t k = (pos == 0) ? 0 : pos + 1;

    /* Compute the term. */
    seq_t result = fib[row + 1] - fib[k];

    free(fib);
    return result;
}
