/*
 * A066799  Square array read by antidiagonals of eventual period of powers of
 *          k mod n; period of repeating digits of 1/n in base k.
 * https://oeis.org/A066799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   not stated
 * Range:     u8 n <= 10011, u16 n <= 10011, u32 n <= 10011, u64 n <= 10011,
 *            u128 n <= 10011, big any n
 * Validated: standard, pass, 120 terms through n = 9536 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A066799: square array read by antidiagonals of eventual periods of powers of k mod n.
 *  a(n) is obtained by first finding the antidiagonal coordinates (row i, column j)
 *  that correspond to index n, and then returning the eventual period of the sequence
 *  k^t (mod i).  The period is computed by iterating the sequence until a value
 *  repeats; the length of the eventual cycle is the answer.
 *
 *  For i = 1 the period is defined to be 1 (the first row of the array consists of 1’s).
 */
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

/* Compute the eventual period of k^t (mod n),  n > 1,  k in [0, n-1] */
static seq_t period_modulo(idx_t n, idx_t k)
{
    if (k == 0)                /* k ≡ 0 (mod n)  →  1,0,0,0,…  */
        return (seq_t)1;

    /* visited[x] holds the step index at which value x first appeared;
       -1 means unseen.  We only need indices up to n-1. */
    idx_t *visited = (idx_t *)malloc(sizeof(idx_t) * n);
    if (!visited)             /* out of memory – unlikely for small n  */
        return (seq_t)0;

    for (idx_t t = 0; t < n; ++t)
        visited[t] = (idx_t)-1;

    idx_t x = 1 % n;
    visited[x] = 0;
    idx_t step = 0;

    for (;;) {
        ++step;
        x = (x * k) % n;
        if (visited[x] != (idx_t)-1) {
            /* found the cycle start at visited[x]; length = step - visited[x] */
            seq_t ret = (seq_t)(step - visited[x]);
            free(visited);
            return ret;
        }
        visited[x] = step;
    }
}

/* Main sequence generator */
seq_t a(idx_t n)
{
    if (n == 0)  /* index starts at 1; return 0 for safety */
        return (seq_t)0;

    /* find diagonal s so that (s-1)*s/2 >= n  and (s-2)*(s-1)/2 < n   */
    idx_t s = 2;
    while ( ((unsigned __int128)(s - 1) * s) / 2 < n )
        ++s;

    /* previous cumulative count up to diagonal s-1 */
    idx_t prev = ((unsigned __int128)(s - 2) * (s - 1)) / 2;
    idx_t pos = n - prev;            /* 1‑based position inside the diagonal */
    idx_t i = pos;                   /* row index   (n in the formula)  */
    idx_t j = s - pos;               /* column index (k in the formula) */

    if (i == 1)                      /* first row → period always 1 */
        return (seq_t)1;

    /* reduce column modulo row because T(i,k)=T(i,k-i) for k>i */
    j %= i;

    return period_modulo(i, j);
}
