/*
 * A124711  Number of base 18 circular n-digit numbers with adjacent digits
 *          differing by 1 or less.
 * https://oeis.org/A124711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A124711 – Number of base‑18 circular n‑digit numbers
 *  with adjacent digits differing by 1 or less.
 *
 *  Formula: a(0) = 1; for n ≥ 1
 *
 *      a(n) = trace(Aⁿ),  where A is an 18 × 18 matrix with
 *      A[i][j] = 1 if |i−j| ≤ 1, else 0.
 *
 *  The sequence satisfies an order‑18 linear recurrence whose
 *  characteristic polynomial is det(λI−A).  The coefficients
 *  of the recurrence are pre‑computed; the first 18 terms are
 *  obtained by repeated matrix multiplication.
 *
 *  The implementation keeps a static cache of computed values.
 *  a(0) is defined to be 1 (the OEIS entry), even though
 *  trace(A⁰) = 18.  For n ≥ 1 only the trace values are used.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define K           18            /* alphabet size (=18)          */
#define MAX_CACHE   10001          /* sufficient for n≤10000      */

static int64_t rec[K + 1];         /* recurrence coefficients r[1..K] */
static seq_t *cache = NULL;        /* cached values a(0)…a(MAX_CACHE-1) */
static size_t cache_len = 0;       /* number of computed entries     */
static bool initialized = false;   /* initialization flag           */

/* ------------------------------------------------------------------ */
/* Build the characteristic polynomial of A and extract recurrence    */
/* coefficients r[j] = –c_j, where c_j are the polynomial coeffs.   */
static void init_recurrence(void)
{
    /* f[m] stores coefficients of f_m(x) = det(xI – A_m),
     * where A_m is the m×m leading principal submatrix.
     * f_0(x)=1; f_1(x)=x–1; f_m(x) = (x–1)f_{m-1}(x) – f_{m-2}(x).
     */
    int64_t f[K + 1][K + 1] = {0};

    f[0][0] = 1;                     /* degree 0 poly */
    f[1][0] = -1; f[1][1] = 1;         /* degree 1 poly : x – 1 */

    for (int m = 2; m <= K; ++m) {
        for (int i = 0; i <= m; ++i) f[m][i] = 0;
        /* (x – 1) · f_{m−1} */
        for (int i = 0; i <= m - 1; ++i) {
            f[m][i + 1] += f[m - 1][i];      /* x·coeff  */
            f[m][i]     += -f[m - 1][i];     /* –1·coeff */
        }
        /* – f_{m−2} */
        for (int i = 0; i <= m - 2; ++i)
            f[m][i] -= f[m - 2][i];
    }

    /* f[K] is the characteristic polynomial  
     * λ^K + c1 λ^{K−1} + … + cK.
     * Recurrence coefficients are r[j] = –c_j = –f[K][K−j].
     */
    for (int j = 1; j <= K; ++j)
        rec[j] = -f[K][K - j];
}

/* ------------------------------------------------------------------ */
/* Compute a(1) … a(K) via repeated multiplication of the adjacency  */
/* matrix A.  Store the trace of Aⁿ in cache[n] for n=1..K.           */
static void init_initial_terms(void)
{
    static uint8_t adj[K][K];
    seq_t mat[K][K], mat_next[K][K];
    seq_t trace = 0;

    /* adjacency matrix: 1 for |i−j|≤1, else 0 */
    for (int i = 0; i < K; ++i)
        for (int j = 0; j < K; ++j)
            adj[i][j] = (i == j || (i > 0 && j == i - 1) || (i + 1 < K && j == i + 1)) ? 1 : 0;

    /* Initialize mat to identity (A⁰) */
    for (int i = 0; i < K; ++i)
        for (int j = 0; j < K; ++j)
            mat[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;

    cache[0] = (seq_t)1;            /* OEIS overrides trace(A⁰)=K */

    /* Compute trace(Aⁿ) for n=1..K */
    for (int step = 1; step <= K; ++step) {
        /* mat_next = mat * adj */
        for (int i = 0; i < K; ++i) {
            for (int j = 0; j < K; ++j) {
                seq_t sum = (seq_t)0;
                for (int k = 0; k < K; ++k) {
                    if (adj[k][j]) sum += mat[i][k] * (seq_t)adj[k][j];
                }
                mat_next[i][j] = sum;
            }
        }
        /* trace */
        trace = (seq_t)0;
        for (int i = 0; i < K; ++i) trace += mat_next[i][i];
        cache[step] = trace;

        /* mat = mat_next for next iteration */
        for (int i = 0; i < K; ++i)
            for (int j = 0; j < K; ++j)
                mat[i][j] = mat_next[i][j];
    }

    cache_len = K + 1;   /* we have computed up to a(K) */
}

/* ------------------------------------------------------------------ */
/* One‑time initialization of all static data structures.             */
static void init_cache(void)
{
    cache = (seq_t *)malloc(sizeof(seq_t) * MAX_CACHE);
    if (!cache) exit(1);          /* unlikely for test data */

    init_recurrence();
    init_initial_terms();

    initialized = true;
}

/* ------------------------------------------------------------------ */
/* Return a(n) for any n≥0.  Uses a cache and a linear recurrence.  */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;

    if (!initialized) init_cache();

    /* Return value if already computed */
    if ((size_t)n < cache_len) return cache[(size_t)n];

    /* Compute new values up to n, using the recurrence */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t sum = (seq_t)0;
        for (int j = 1; j <= K; ++j) {
            seq_t term = cache[i - j];
            seq_t mul  = term * (seq_t)(rec[j] >= 0 ? rec[j] : -rec[j]);
            if (rec[j] >= 0)
                sum += mul;
            else
                sum -= mul;
        }
        cache[i] = sum;
    }

    cache_len = (size_t)n + 1; /* update computed length */
    return cache[(size_t)n];
}
