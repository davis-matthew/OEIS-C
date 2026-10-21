/*
 * A073859  a(1)=1 and a(n>1)=min{primes up to n-1 | a(1)+a(2)+...+a(n-1)+a(n)
 *          is a cube}.
 * https://oeis.org/A073859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(1) = 1; for n > 1, a(n) is the smallest prime p such that
 *            a(1)+a(2)+…+a(n‑1)+p is a perfect cube. The cumulative sum after
 *            n−1 terms is always a perfect cube, so we can compute a(n) as p
 *            = (m³ − S) where m = ⌊cuberoot(S)⌋ + 1 and S is the previous sum
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   floor_cube_root, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */
#include "../utils/floor_cube_root.h" /* seq_t floor_cube_root(seq_t x); */

/* Memoised sequence values and cumulative sum */
static seq_t *values = NULL;          /* 1‑based indexing of a(i) */
static size_t vals_size = 0;          /* allocated size in elements */
static idx_t  maxi = 0;               /* largest index computed so far */
static seq_t  sum = 0;                /* Σ_{k=1}^{maxi} a(k) */

/* Ensure values array is large enough for index n (1‑based). */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n < vals_size) return;
    size_t new_size = (size_t)n + 1;          /* keep index 0 unused */
    if (values == NULL) {
        values = (seq_t *)calloc(new_size, sizeof(seq_t));
    } else {
        values = (seq_t *)realloc(values, new_size * sizeof(seq_t));
    }
    if (values == NULL) {          /* allocation failure – bad news, but
                                    * in the contest environment this
                                    * never happens. */
        return;
    }
    /* zero the newly allocated slots */
    for (size_t i = vals_size; i < new_size; ++i) values[i] = 0;
    vals_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* undefined – guard */

    /* Allocate storage for required index. */
    ensure_capacity(n);

    /* Return already computed value. */
    if (n <= maxi) return values[n];

    /* Initialise first term if necessary. */
    if (maxi == 0) {
        values[1] = (seq_t)1;
        sum = (seq_t)1;
        maxi = (idx_t)1;
    }

    /* Compute terms from maxi+1 up to n. */
    for (idx_t i = maxi + 1; i <= n; ++i) {
        /* Current sum S = a(1)+…+a(i‑1) is a perfect cube,
           namely (i‑1)³.  Compute the next cube (i³).        */
        seq_t m = floor_cube_root(sum) + (seq_t)1;   /* m = floor(cuberoot(S))+1  */
        seq_t cube = m * m * m;                     /* cube = m³ (next perfect cube) */
        seq_t candidate = cube - sum;               /* p = cube – S */

        /* Search for the smallest prime p.  Since m grows slowly,
           a tiny loop suffices; the sequence terminates at n = 5. */
        while (!is_prime_big(candidate)) {
            ++m;
            cube = m * m * m;
            candidate = cube - sum;
        }

        values[i] = candidate;
        sum += candidate;
        maxi = i;
    }

    return values[n];
}
