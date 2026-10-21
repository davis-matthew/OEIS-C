/*
 * A211782  Rectangular array: R(n,k)=[n/F(2)]+[n/F(3)]+...+[n/F(k+1)], where
 *          [ ]=floor and F=A000045 (Fibonacci numbers), by antidiagonals.
 * https://oeis.org/A211782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Σ_{i=2}^{k+1}⌊r / F(i)⌋, * where (r,k) is the pair
 *            obtained by enumerating the rectangular * array
 *            R(r,k)=Σ_{i=2}^{k+1}⌊r/F(i)⌋ along antidiagonals (offset‑1). *
 *            This implements the OEIS definition of A211782
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

static idx_t fibbuf[1024];
static size_t fiblen = 0;

/* Ensure Fibonacci numbers up to the given (1‑based) index are computed. */
static void ensure_fib(size_t limit)
{
    if (fiblen == 0) {
        fibbuf[1] = 1;
        fibbuf[2] = 1;
        fiblen = 2;
    }
    if (limit <= fiblen) return;
    if (limit > 1024) limit = 1024;          /* safety, never needed for n≤1e4 */
    for (size_t i = fiblen + 1; i <= limit; ++i) {
        fibbuf[i] = fibbuf[i-1] + fibbuf[i-2];
    }
    fiblen = limit;
}

/* Sequence A211782 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset 1; n≥1 in practice */

    /* Find the antidiagonal d such that cumulative count >= n. */
    idx_t d   = 1;
    idx_t cum = 0;
    do {
        ++d;
        cum += d - 1;                     /* length of diagonal d = d‑1 */
    } while (cum < n);

    /* Offset within diagonal d (0‑based). */
    idx_t prev_cum = cum - (d - 1);
    idx_t offset   = (n - 1) - prev_cum;

    /* Map to row r and column k. */
    idx_t row = (d - 1) - offset;
    idx_t col = offset + 1;

    /* Compute the sum Σ_{i=2}^{k+1} ⌊row / F(i)⌋. */
    ensure_fib(col + 1);                  /* need Fibonacci up to F(col+1) */
    seq_t sum = 0;
    for (idx_t i = 2; i <= col + 1; ++i) {
        sum += (seq_t)row / fibbuf[i];
    }
    return sum;
}
