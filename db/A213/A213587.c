/*
 * A213587  Rectangular array: (row n) = b**c, where b(h) = F(h+1), c(h) =
 *          F(n+h), F = A000045 (Fibonacci numbers), n>=1, h>=1, and ** =
 *          convolution.
 * https://oeis.org/A213587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A213587: Rectangular array convolution of Fibonacci numbers.
 * T(n,k) = (k*Lucas(n+k+2) - Fibonacci(k)*Lucas(n-1)) / 5
 * The sequence a(n) is read antidiagonally from the array T(n,k)
 * (first element is T(1,1), then T(1,2),T(2,1), etc.).
 */

#include <stdlib.h>

/* Fibonacci cache: fib_cache[i] = F(i).  */
static seq_t *fib_cache = NULL;
static idx_t fib_len   = 0;   /* number of entries in fib_cache */

/* Ensure fib_cache contains Fibonacci numbers up to index 'upto' (inclusive). */
static void ensure_fib(idx_t upto)
{
    /* fib_cache currently holds indices 0 .. fib_len-1 */
    if (upto + 1 <= fib_len) return;

    size_t new_len = (size_t)upto + 1;
    seq_t *tmp = (seq_t *)realloc(fib_cache, new_len * sizeof(seq_t));
    if (!tmp) abort();
    fib_cache = tmp;

    if (fib_len == 0) {
        fib_cache[0] = (seq_t)0;
        fib_cache[1] = (seq_t)1;
        fib_len = 2;
    }

    for (idx_t i = fib_len; i <= upto; ++i)
        fib_cache[i] = fib_cache[i-1] + fib_cache[i-2];

    fib_len = new_len;
}

/* Lucas number L(n) for n >= 0: L(0)=2, L(n)=F(n-1)+F(n+1) for n>=1. */
static inline seq_t lucas_idx(idx_t n)
{
    if (n == 0) return (seq_t)2;
    /* Need F(n-1) and F(n+1).  */
    ensure_fib(n + 1);
    return fib_cache[n-1] + fib_cache[n+1];
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n<1, return 0 */

    /* Find the antidiagonal (row index s) that contains the n-th term. */
    idx_t s   = 0;
    idx_t sum = 0;
    while (sum < n) {
        ++s;
        sum += s;
    }

    /* Position within the diagonal (1‑based). */
    idx_t prev = sum - s;   /* sum of first s-1 diagonals = s(s-1)/2 */
    idx_t pos  = n - prev;  /* 1 .. s */

    /* Map to matrix coordinates: (row, col). */
    idx_t row = pos;                /* n-coordinate (row) */
    idx_t col = s - pos + 1;        /* k-coordinate (col) */

    /* Needed Fibonacci indices for Lucas numbers. */
    idx_t need_fib = row + col + 3;   /* up to F(row+col+3) for Lucas(row+col+2) */
    ensure_fib(need_fib);

    /* Retrieve components of the formula. */
    seq_t fibk   = fib_cache[col];              /* Fibonacci(k)   */
    seq_t lucnk  = lucas_idx(row + col + 2);    /* Lucas(n+k+2)   */
    seq_t lucn1  = lucas_idx(row - 1);          /* Lucas(n-1)     */

    /* Compute T(n,k). */
    seq_t numer = ((seq_t)col) * lucnk - fibk * lucn1;
    return numer / (seq_t)5;
}
