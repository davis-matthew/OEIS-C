/*
 * A196840  Duplicate of A162298.
 * https://oeis.org/A196840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = numerator of the rational number * r(k,m) =
 *            (B_{k+1-m}/(k+1-m)) * C(k,m) * where (k,m) satisfy n = k*(k+1)/2
 *            + (m-1), * B_j are Bernoulli numbers, and C(k,m) is the binomial
 *            coefficient
 * Range:     i8 none, i16 n <= 54, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/binom.h"

#define MAX_K 200

static seq_t Bnum[MAX_K + 1];
static seq_t Bden[MAX_K + 1];
static bool Binit = false;

/* Euclidean gcd for seq_t (handles negative arguments). */
static seq_t my_gcd(seq_t a, seq_t b)
{
    if (a < (seq_t)0) a = -a;
    if (b < (seq_t)0) b = -b;
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Generate Bernoulli numbers B_0 … B_MAX_K as reduced numerators
 * and denominators using the Akiyama–Tanigawa algorithm. */
static void init_bernoulli(void)
{
    seq_t *A_num = (seq_t *)malloc((MAX_K + 1) * sizeof(seq_t));
    seq_t *A_den = (seq_t *)malloc((MAX_K + 1) * sizeof(seq_t));

    /* Initial values: A[i] = 1/(i+1). */
    for (int i = 0; i <= MAX_K; ++i) {
        A_num[i] = 1;
        A_den[i] = (idx_t)(i + 1);
    }

    for (int m = 0; m <= MAX_K; ++m) {
        /* Set A[m] = 1/(m+1) for this m. */
        A_num[m] = 1;
        A_den[m] = (idx_t)(m + 1);

        for (int j = m; j >= 1; --j) {
            /* Difference: A[j-1] = j * (A[j-1] - A[j]) */
            seq_t num = A_num[j - 1] * A_den[j] - A_num[j] * A_den[j - 1];
            seq_t den = A_den[j - 1] * A_den[j];

            seq_t g = my_gcd(num >= (seq_t)0 ? num : -num, den);
            num /= g;
            den /= g;

            num *= (seq_t)j;
            g = my_gcd(num >= (seq_t)0 ? num : -num, den);
            num /= g;
            den /= g;

            A_num[j - 1] = num;
            A_den[j - 1] = den;
        }

        Bnum[m]   = A_num[0];
        Bden[m]   = A_den[0];
        if (m == 1) {
            Bnum[m] = -Bnum[m];
        }
    }

    free(A_num);
    free(A_den);
}

/* Convert flattened index n to triangle coordinates (k,m)
 * with 0‑based k and 1‑based m. */
static void index_to_coords(idx_t n, idx_t *k, idx_t *m)
{
    idx_t kk = 0;
    while ((kk + 1) * (kk + 2) / 2 <= n) {
        ++kk;
    }
    *k = kk;
    idx_t row_start = kk * (kk + 1) / 2;
    *m = n - row_start + 1;   /* 1 … k+1 */
}

/* Public function requested in the problem statement. */
seq_t a(idx_t n)
{
    if (!Binit) {
        init_bernoulli();
        Binit = true;
    }

    idx_t k, m;
    index_to_coords(n, &k, &m);

    if (m == k + 1 || m == k) {
        /* r(k,k+1) = 1/(k+1), r(k,k) = 1/2 → numerator 1. */
        return (seq_t)1;
    }

    idx_t t = k + 1 - m;               /* 2 … k */
    int tr = (int)t;                   /* t ≤ MAX_K for n ≤ 10000 */

    seq_t Bn = Bnum[tr];
    seq_t Bd = Bden[tr];

    if (Bn == (seq_t)0) {
        return (seq_t)0;
    }

    seq_t C = binom(k, m);

    seq_t num = Bn * C;
    seq_t den = Bd * (seq_t)t;

    seq_t g = my_gcd(num >= (seq_t)0 ? num : -num, den);
    num /= g;
    return num;
}
