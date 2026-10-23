/*
 * A166898  G.f.: exp( Sum_{n>=1} [Sum_{k=0..n} C(n,k)^4 * x^k] * x^n/n ), an
 *          integer series in x.
 * https://oeis.org/A166898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   alloc_seq_array, binom_n_k, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* University of Vienna – OEIS A166898
   Sequence defined by the ordinary generating function
   A(x) = exp( Sum_{n>=1} A166899(n)*x^n/n )      (A166899 defined below)
   Recurrence:
     a(0) = 1
     a(n) = (1/n) * Sum_{k=1..n} A166899(k) * a(n-k)    for n >= 1
   with
     A166899(n) = Sum_{k=0..floor(n/2)} C(n-k,k)^4 * n/(n-k)
   The function below implements this recurrence in an
   incrementally local‑memory cache.  It works for any build
   where seq_t is an unsigned integer type (8, 16, 32, 64, 128
   bits or an arbitrary‑precision integer class) and idx_t
   is uint64_t.  All arithmetic is performed using seq_t
   regardless of the concrete type.  Allocation of the
   cache uses realloc, which requires an explicit cast in
   C++ builds. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binom_n_k.h"
#include "../utils/seq_pow.h"
#include "../utils/alloc_seq_array.h"

static idx_t   _max_n = 0;   /* largest n that has been computed */
static seq_t * _a     = NULL;/* sequence a(n) */
static seq_t * _b     = NULL;/* auxiliary sequence A166899(n) */

/* Public entry point: a(n) */
seq_t a(idx_t n)
{
    /* If the cache has not been initialised yet, do it now. */
    if (_a == NULL) {
        size_t initsize = (size_t)n + 1;
        _a = alloc_seq_array(initsize);     /* zero‑initialised by alloc_seq_array */
        _b = alloc_seq_array(initsize);
        _a[0] = (seq_t)1;    /* a(0) = 1 */
        _b[0] = (seq_t)0;    /* unused but harmless */
        _max_n = 0;
    }

    /* If the requested n is larger than what we have cached, extend the cache. */
    if (n > _max_n) {
        size_t newsize = (size_t)n + 1;
        /* Reallocate the arrays to fit up to n */
        _a = (seq_t*)realloc(_a, newsize * sizeof(seq_t));
        _b = (seq_t*)realloc(_b, newsize * sizeof(seq_t));

        /* Compute A166899 for new indices */
        for (idx_t i = _max_n + 1; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            idx_t maxk = i / 2;
            for (idx_t k = 0; k <= maxk; ++k) {
                idx_t nmk = i - k;
                seq_t bin = binom_n_k(nmk, k);                 /* C(i-k, k) */
                seq_t bin4 = seq_pow(bin, (idx_t)4);           /* (C(i-k, k))^4 */
                seq_t mult = bin4 * (seq_t)i;                 /* multiply by i */
                seq_t term = mult / (seq_t)nmk;               /* divide by (i-k) */
                sum += term;
            }
            _b[i] = sum;    /* _b[i] = A166899(i) */
        }

        /* Compute a(n) for the new indices */
        for (idx_t m = _max_n + 1; m <= n; ++m) {
            seq_t sum = (seq_t)0;
            for (idx_t j = 1; j <= m; ++j) {
                sum += _a[m - j] * _b[j];
            }
            _a[m] = sum / (seq_t)m;   /* a(m) = sum / m */
        }

        _max_n = n;
    }

    return _a[n];
}
