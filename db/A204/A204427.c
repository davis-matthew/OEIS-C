/*
 * A204427  Infinite matrix: f(i,j)=(2i+j+2 mod 3), read by antidiagonals.
 * https://oeis.org/A204427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (2*i + j + 2) mod 3, where (i,j) is the 0‑based *
 *            position of the n‑th term read along antidiagonals of the matrix
 *            * f(i,j) = (2*i + j + 2) mod 3. For n ≥ 1 the algorithm runs in
 *            * O(√n) time
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the nth term of A204427 */
seq_t a(idx_t n)
{
    if (n == 0)  /* offset is 1, behaviour for n==0 is irrelevant */
        return (seq_t)0;

    /* Find the antidiagonal index s (i + j = s) containing term n. */
    idx_t s = 0;             /* current diagonal index */
    idx_t cum = 0;           /* total terms before diagonal s */

    /* Increment s until cum + (s+1) >= n */
    while (cum + (s + 1) < n) {
        cum += s + 1;
        ++s;
    }

    /* Offset d within diagonal s (0 ≤ d ≤ s). */
    idx_t d = n - cum - 1;

    /* Coordinates in the matrix (0‑based). */
    idx_t i = d;
    idx_t j = s - d;

    /* Compute the value f(i,j) = (2*i + j + 2) mod 3. */
    idx_t tmp = 2 * i + j + 2;
    /* tmp fits in idx_t; reduce modulo 3 to obtain 0,1,2. */
    tmp %= 3u;

    return (seq_t)tmp;
}
