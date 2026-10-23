/*
 * A125329  Number of base 19 circular n-digit numbers with adjacent digits
 *          differing by 3 or less.
 * https://oeis.org/A125329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base 19 circular n‑digit numbers whose adjacent
 *            digits differ by at most 3. Computation is done by forming the
 *            19×19 adjacency matrix M_ij = 1 if |i−j| ≤ 3, else 0. The
 *            sequence is a(n) = trace(Mⁿ) for n ≥ 1 with a(0) = 1. The
 *            recurrence of order 19 is obtained from the character...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stddef.h>
#include <stdlib.h>

/* Matrix dimension */
#define N 19

/* Helper to compute absolute difference of small integers */
static inline int abs_diff(int a, int b)
{
    return a > b ? a - b : b - a;
}

/* Multiply two N×N matrices: C = A × B.
   All entries are of type seq_t. */
static void mat_mul(seq_t const A[N][N], seq_t const B[N][N], seq_t C[N][N])
{
    int i, j, k;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            seq_t sum = 0;
            for (k = 0; k < N; k++)
            {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

/* Compute trace of an N×N matrix. */
static seq_t mat_trace(seq_t const M[N][N])
{
    int i;
    seq_t sum = 0;
    for (i = 0; i < N; i++)
        sum += M[i][i];
    return sum;
}

/* Global static data: recurrence coefficients and cache. */
static seq_t coeff[N + 1];          /* coeff[1] … coeff[N] */
static seq_t *cache = NULL;        /* cache[i] = a(i) */
static size_t cache_size = 0;      /* number of entries currently allocated */
static int initialized = 0;        /* flag */

/* One‑time initialization: build adjacency matrix, compute
   a(0)..a(N), and recurrence coefficients. */
static void init_once(void)
{
    int i, j, k;
    seq_t M[N][N];
    seq_t pow[N][N];
    seq_t trace_vals[N + 1];   /* trace_vals[0] unused (a(0)=1) */

    /* Build adjacency matrix M */
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            M[i][j] = abs_diff(i, j) <= 3 ? 1 : 0;
        }
    }

    /* Compute a(0)..a(N). a(0)=1. For n≥1, a(n)=trace(M^n). */
    cache = (seq_t *)calloc(N + 1, sizeof(seq_t));
    cache[0] = 1; /* a(0) */
    cache_size = N;

    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            pow[i][j] = M[i][j];          /* pow = M^1 */

    for (k = 1; k <= N; k++)
    {
        cache[k] = mat_trace(pow);
        /* Prepare next power: pow = pow × M */
        seq_t next[N][N];
        mat_mul(pow, M, next);
        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++)
                pow[i][j] = next[i][j];
    }

    /* Compute recurrence coefficients via Newton identities. */
    /* s_k = cache[k] for k = 1..N */
    for (k = 1; k <= N; k++)
    {
        seq_t sum = cache[k];  /* s_k */
        for (j = 1; j < k; j++)
            sum += coeff[j] * cache[k - j];
        /* sum is divisible by k */
        seq_t div = sum / (seq_t)k;
        coeff[k] = -div;
    }

    initialized = 1;
}

/* Main function to compute a(n) */
seq_t a(idx_t n)
{
    /* Ensure initialization once */
    if (!initialized)
        init_once();

    /* Extend cache if necessary */
    if (n > cache_size)
    {
        size_t old_size = cache_size;
        size_t new_size = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        for (size_t i = old_size; i <= n; i++)
        {
            seq_t sum = 0;
            for (int j = 1; j <= N; j++)
                sum += coeff[j] * cache[i - j];
            cache[i] = -sum;
        }
        cache_size = n;
    }

    return cache[(size_t)n];
}
