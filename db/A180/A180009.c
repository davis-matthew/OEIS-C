/*
 * A180009  Triangle T(n,k), read by rows, T(n,1) = mu(n), T(n,2) = T(n,k-1) -
 *          T(n-1,k), k > 2, T(n,k) = Sum_{i=1..k-1} ( T(n-i,k-1) - T(n-i,k)
 *          ).
 * https://oeis.org/A180009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   Triangle T(n,k) with T(n,1) = μ(n) (Möbius function) T(n,2) =
 *            T(n,1) − T(n-1,2) for n ≥ 2 T(n,k) = Σ_{i=1}^{k-1} (T(n-i,k-1) −
 *            T(n-i,k)) for k ≥ 3 Sequence a(n) is obtained by reading the
 *            triangle by rows, so that a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2),
 *            a(4)=T(3,1), …
 * Range:     i8 n <= 5050, i16 n <= 5050, i32 n <= 5050, i64 n <= 5050,
 *            i128 n <= 5050, big any n
 * Helpers:   mobius_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>   /* for calloc */
#include "../utils/mobius_sieve.h"

seq_t a(idx_t n)
{
    /* Locate the row and column that contains the n‑th term */
    idx_t row = 1;
    idx_t cumulative = 1;
    while (cumulative < n)
    {
        ++row;
        cumulative += row;
    }
    idx_t prev_cum = cumulative - row;
    idx_t col = n - prev_cum;   /* 1‑based column inside the row */

    /* Allocate temporary arrays for Möbius values and the triangle */
    size_t r = (size_t)row;
    int *mu    = (int *)calloc(r + 1, sizeof(int));
    int *spf   = (int *)calloc(r + 1, sizeof(int));
    int *primes = (int *)calloc(r + 1, sizeof(int));
    size_t pc = 0;
    mobius_sieve(r, mu, spf, primes, &pc);

    size_t width = r + 1;
    size_t len   = width * width;
    seq_t *T = (seq_t *)calloc(len, sizeof(seq_t));

    /* helper macro to access element (i,k)   (i,k >=1) */
#define IDX(i, k) ((size_t)(i) * width + (size_t)(k))

    /* build the triangle row by row */
    for (idx_t i = 1; i <= r; ++i)
    {
        T[IDX(i, 1)] = (seq_t)mu[i];

        if (i >= 2)
        {
            T[IDX(i, 2)] = T[IDX(i, 1)] - T[IDX(i - 1, 2)];
        }

        for (idx_t k = 3; k <= i; ++k)
        {
            seq_t sum = 0;
            for (idx_t j = 1; j <= k - 1; ++j)
            {
                /* If i-j < 1, the corresponding T is 0 (array entry is 0) */
                seq_t a1 = (i > j) ? T[IDX(i - j, k - 1)] : 0;
                seq_t a2 = (i > j) ? T[IDX(i - j, k)]     : 0;
                sum += a1 - a2;
            }
            T[IDX(i, k)] = sum;
        }
    }

    seq_t result = T[IDX(row, col)];

    free(mu);
    free(spf);
    free(primes);
    free(T);
    return result;
}
