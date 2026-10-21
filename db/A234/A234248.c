/*
 * A234248  Number of distinct lines passing through at least three points in
 *          a triangular grid of side n.
 * https://oeis.org/A234248
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = 3*Σ_{j=1..⌊(n-1)/2⌋} φ(j) * (g(n-2j) - g(n-3j)) where
 *            φ(j) is Euler's totient function and g(i)=i*(i+1)/2 (triangular
 *            number) with g(i)=0 for i≤0. Offset: 3 (a(3)=3)
 * Range:     u8 n <= 11, u16 n <= 49, u32 n <= 798, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Euler's totient cache                                                   */

static idx_t *phi_cache = NULL;   /* array of φ values up to the current limit */
static idx_t phi_max   = 0;        /* maximum j that has been computed */

static void compute_phi_upto(idx_t limit)
{
    if (limit <= phi_max) return;                /* already up to date */
    free(phi_cache);                             /* shrink previous old data */
    phi_cache = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    for (idx_t i = 0; i <= limit; ++i) phi_cache[i] = i;
    for (idx_t p = 2; p <= limit; ++p)
        if (phi_cache[p] == p)                    /* p is prime */
            for (idx_t j = p; j <= limit; j += p)
                phi_cache[j] -= phi_cache[j] / p;
    phi_max = limit;
}

/* ------------------------------------------------------------------ */
/* triangular number g(i)                                                */

static inline seq_t tri(idx_t i)
{
    return i == 0 ? (seq_t)0 : ((seq_t)i * ((seq_t)i + (seq_t)1)) / (seq_t)2;
}

/* ------------------------------------------------------------------ */
/* a(n) : number of distinct lines through at least three points in a triangular
   grid of side n (offset 3)                                       */

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;                 /* below offset, undefined */

    idx_t max_j = (n - 1) / 2;                  /* floor((n-1)/(k-1)), k=3 */
    compute_phi_upto(max_j);

    seq_t sum = 0;
    for (idx_t j = 1; j <= max_j; ++j)
    {
        seq_t t2 = tri(n - (idx_t)(2 * j));          /* g(n-2j) */
        seq_t t3 = 0;
        if (n > (idx_t)(3 * j))
            t3 = tri(n - (idx_t)(3 * j));          /* g(n-3j) only if positive */
        seq_t diff = t2 - t3;                       /* g(n-2j) - g(n-3j) */
        sum += (seq_t)phi_cache[j] * diff;
    }
    return sum * (seq_t)3;
}
