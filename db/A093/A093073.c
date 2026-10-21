/*
 * A093073  McKay-Thompson series of class 18A for the Monster group with a(0)
 *          = -1.
 * https://oeis.org/A093073
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of q^(n+1) in eta(q) * eta(q^2) / (eta(q^9) *
 *            eta(q^18))
 * Range:     i8 n <= 72, i16 n <= 216, i32 n <= 711, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#if defined(__cplusplus)
#  include <cstdlib>
#  define ALLOC(type, count)  new type[(count)]
#  define FREE(ptr)          delete[] (ptr)
#else
#  define ALLOC(type, count) ((type*)malloc((count) * sizeof(type)))
#  define FREE(ptr)          free(ptr)
#endif

/* ------------------------------------------------------------------ */
/* Compute the eta series without the q^{1/24} prefactor, truncated to
   degree N (inclusive).  out[0] must be set to 1 before calling. */
static void eta_series_no_prefactor(seq_t *out, size_t N, int step)
{
    for (size_t s = step; s <= N; s += step) {
        for (size_t i = N; i >= (size_t)s; --i) {
            out[i] -= out[i - s];
        }
    }
}

/* Allocate and compute the eta series for a given step. */
static seq_t *compute_eta(int step, size_t N)
{
    seq_t *arr = ALLOC(seq_t, N + 1);
    if (!arr) return NULL;
    arr[0] = 1;
    for (size_t i = 1; i <= N; ++i) arr[i] = 0;
    eta_series_no_prefactor(arr, N, step);
    return arr;
}

/* Convolution of a and b up to degree N into out.  out must be
   allocated with size N+1.  a[0] and b[0] are assumed non‑zero. */
static void convolve(const seq_t *a, const seq_t *b, seq_t *out, size_t N)
{
    for (size_t i = 0; i <= N; ++i) out[i] = 0;
    for (size_t i = 0; i <= N; ++i) {
        seq_t ai = a[i];
        if (ai == 0) continue;
        for (size_t j = 0; i + j <= N; ++j) {
            out[i + j] += ai * b[j];
        }
    }
}

/* Compute the power series S(q) = eta(q)*eta(q^2)/(eta(q^9)*eta(q^18))
   up to degree N (inclusive).  Result is stored in out[0..N]. */
static void compute_S_series(seq_t *out, size_t N)
{
    seq_t *e1  = compute_eta(1,  N);
    seq_t *e2  = compute_eta(2,  N);
    seq_t *e9  = compute_eta(9,  N);
    seq_t *e18 = compute_eta(18, N);
    if (!e1 || !e2 || !e9 || !e18) return;

    seq_t *num = ALLOC(seq_t, N + 1);
    seq_t *den = ALLOC(seq_t, N + 1);
    if (!num || !den) {
        FREE(e1); FREE(e2); FREE(e9); FREE(e18);
        FREE(num); FREE(den);
        return;
    }

    convolve(e1, e2, num, N);
    convolve(e9, e18, den, N);

    out[0] = 1;
    for (size_t n = 1; n <= N; ++n) {
        seq_t sum = 0;
        for (size_t k = 1; k <= n; ++k) sum += den[k] * out[n - k];
        out[n] = num[n] - sum;
    }

    FREE(e1); FREE(e2); FREE(e9); FREE(e18);
    FREE(num); FREE(den);
}

/* ------------------------------------------------------------------ */
/* Cache for a(n) from n = -1 up to the largest requested n. */
static seq_t *a_cache = NULL;
static size_t a_cache_len = 0;          /* actual allocated length */
static idx_t a_cache_max_n = -2;       /* largest n cached (-2 → none) */

/* Expand the cache to include all values up to n (n ≥ -1). */
static void ensure_cache(idx_t n)
{
    if (n < -1) return;
    if (n <= a_cache_max_n) return;                 /* already cached */

    size_t new_len = (size_t)n + 2;                  /* indices 0..new_len-1; pos=n+1 */
    seq_t *new_a = ALLOC(seq_t, new_len);
    if (!new_a) return;                             /* allocation failure */

    /* Compute the S series up to degree N = n+1, because a(n) = S[n+1]. */
    size_t N = n + 1;
    seq_t *S = ALLOC(seq_t, N + 1);
    if (!S) {
        FREE(new_a);
        return;
    }
    compute_S_series(S, N);

    /* Copy S into cache: pos = k corresponds to a(n=k-1). */
    for (size_t pos = 0; pos <= N; ++pos) {
        new_a[pos] = S[pos];
    }
    FREE(S);

    /* Replace old cache.  Freeing a NULL pointer is safe. */
    FREE(a_cache);
    a_cache = new_a;
    a_cache_len = new_len;
    a_cache_max_n = n;
}

/* ------------------------------------------------------------------ */
/* Public function as required. */
seq_t a(idx_t n)
{
    if (n < -1) return 0;            /* below offset – undefined */
    ensure_cache(n);
    return a_cache[(size_t)n + 1];
}
