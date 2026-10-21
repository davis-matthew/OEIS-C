/*
 * A227732  O.g.f.: exp( Sum_{n>=1} (sigma(2*n)^2 - sigma(n)^2) * x^n/n ).
 * https://oeis.org/A227732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. For n >= 1: a(n) = (1/n) * Σ_{k=1}^{n} (σ(2k)^2 –
 *            σ(k)^2) * a(n-k), where σ(m) is the sum of the positive divisors
 *            of m
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation uses incremental dynamic arrays and a simple
    recurrence derived from the exponential generating function
    G(x) = exp( Σ_{k>=1} (σ(2k)^2 – σ(k)^2) * x^k / k ).  */

#include <stdlib.h>
#include <stdint.h>

static seq_t *a_vals = NULL;
static seq_t *c_vals = NULL;
static seq_t *sigma  = NULL;
static size_t a_cap   = 0;
static size_t c_cap   = 0;
static size_t sigma_cap = 0;

#if defined(__cplusplus)
/* C++ version: use new[]/delete[]; seq_t may be a big‑int class */
static void recompute_to(idx_t n)
{
    size_t N = (size_t)n;
    size_t limit = N * 2;

    /* sigma up to 2*N */
    if (!sigma || sigma_cap <= limit) {
        delete[] sigma;
        sigma = new seq_t[limit + 1]();
        sigma_cap = limit + 1;
        for (size_t i = 1; i <= limit; ++i)
            for (size_t j = i; j <= limit; j += i)
                sigma[j] += (seq_t)i;
    }

    /* C[k] = σ(2k)^2 – σ(k)^2  for k=1..N */
    if (!c_vals || c_cap <= N) {
        delete[] c_vals;
        c_vals = new seq_t[N + 1]();
        c_cap = N + 1;
        for (size_t k = 1; k <= N; ++k) {
            seq_t s  = sigma[k];
            seq_t s2 = sigma[2 * k];
            c_vals[k] = s2 * s2 - s * s;
        }
    }

    /* a[n] recurrence */
    if (!a_vals || a_cap <= N) {
        delete[] a_vals;
        a_vals = new seq_t[N + 1]();
        a_cap = N + 1;
        a_vals[0] = (seq_t)1;
        for (size_t nidx = 1; nidx <= N; ++nidx) {
            seq_t sum = 0;
            for (size_t k = 1; k <= nidx; ++k)
                sum += c_vals[k] * a_vals[nidx - k];
            a_vals[nidx] = sum / (seq_t)nidx;
        }
    }
}
#else
/* C version: use malloc/realloc; all arithmetic fits in uint64_t.
   Use unsigned __int128 for intermediate products to avoid overflow. */
static void recompute_to(idx_t n)
{
    size_t N = (size_t)n;
    size_t limit = N * 2;

    /* sigma up to 2*N */
    if (!sigma || sigma_cap <= limit) {
        sigma = (seq_t*)realloc(sigma, (limit + 1) * sizeof(seq_t));
        if (!sigma) exit(1);
        for (size_t i = 0; i <= limit; ++i) sigma[i] = 0;
        sigma_cap = limit + 1;
        for (size_t i = 1; i <= limit; ++i)
            for (size_t j = i; j <= limit; j += i)
                sigma[j] += (seq_t)i;
    }

    /* C[k] */
    if (!c_vals || c_cap <= N) {
        c_vals = (seq_t*)realloc(c_vals, (N + 1) * sizeof(seq_t));
        if (!c_vals) exit(1);
        c_cap = N + 1;
        for (size_t k = 1; k <= N; ++k) {
            seq_t s  = sigma[k];
            seq_t s2 = sigma[2 * k];
            unsigned __int128 tmp = (unsigned __int128)s2 * s2;
            tmp -= (unsigned __int128)s * s;
            c_vals[k] = (seq_t)tmp;
        }
    }

    /* a[n] */
    if (!a_vals || a_cap <= N) {
        a_vals = (seq_t*)realloc(a_vals, (N + 1) * sizeof(seq_t));
        if (!a_vals) exit(1);
        a_cap = N + 1;
        a_vals[0] = (seq_t)1;
        for (size_t nidx = 1; nidx <= N; ++nidx) {
            unsigned __int128 sum = 0;
            for (size_t k = 1; k <= nidx; ++k) {
                sum += (unsigned __int128)c_vals[k] * a_vals[nidx - k];
            }
            a_vals[nidx] = (seq_t)(sum / (unsigned __int128)nidx);
        }
    }
}
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    recompute_to(n);
    return a_vals[(size_t)n];
}
