/*
 * A109413  a(n) equals the (n*(n+1)/2)-th partial sum of the self-convolution
 *          of A010054, which has the g.f.: Sum_{n>=0} x^(n*(n+1)/2).
 * https://oeis.org/A109413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^(n*(n+1)/2)] (Sum_{k>=0} x^(k*(k+1)/2))^2/(1-x)
 * Range:     u8 n <= 17, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence counts ordered pairs (i,j) of triangular numbers t_i + t_j
   that do not exceed the triangular number T_n = n*(n+1)/2. All values
   are computed with the build‑provided types seq_t and idx_t. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Triangular numbers cache: t_k = k*(k+1)/2, 0 <= k. */
static unsigned long long *Tri = NULL;  /* array of unsigned long long */
static size_t Tri_len = 0;              /* number of elements currently stored */
static size_t Tri_cap = 0;              /* allocated capacity */

/* Ensure that Tri contains t_k for 0 <= k <= n. */
static void ensure_tri(size_t n)
{
    /* If already stored, nothing to do. */
    if (Tri_len > n)
        return;

    /* Grow the array if necessary. */
    size_t new_cap = Tri_cap ? Tri_cap : 32;
    while (new_cap <= n)
        new_cap <<= 1;

    unsigned long long *new_tri = (unsigned long long *)realloc(Tri, new_cap * sizeof(unsigned long long));
    if (!new_tri && new_cap > 0)
        abort(); /* allocation failed */

    Tri = new_tri;
    Tri_cap = new_cap;

    /* Fill the new entries. */
    for (size_t k = Tri_len; k <= n; ++k)
        Tri[k] = ((unsigned long long)k * (k + 1ULL)) / 2ULL;

    Tri_len = n + 1;
}

seq_t a(idx_t n)
{
    /* Convert n safely to seq_t for arithmetic. */
    seq_t seq_n = (seq_t)n;

    /* Triangular number T_n = n*(n+1)/2. Use unsigned long long
       as n <= 10000 (and even larger is harmless here). */
    unsigned long long Tn = ((unsigned long long)n * (n + 1ULL)) / 2ULL;

    /* Make sure we have triangular numbers up to n. */
    ensure_tri((size_t)n);

    seq_t total = 0;

    /* Two‑pointer technique: for each i, find the largest j
       such that t_i + t_j <= Tn. */
    size_t j = n;
    for (size_t i = 0; i <= (size_t)n; ++i) {
        /* Decrease j until the sum fits. */
        while (j > 0 && Tri[i] + Tri[j] > Tn)
            --j;
        /* Add the number of j's (0..j inclusive). */
        total += (seq_t)(j + 1);
    }

    return total;
}
