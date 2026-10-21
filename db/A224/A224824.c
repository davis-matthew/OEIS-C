/*
 * A224824  Smallest m such that Fibonacci(m) >= m^n.
 * https://oeis.org/A224824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   fib, fib_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = smallest m ≥ 2 such that Fibonacci(m) ≥ m^n
 *
 * The algorithm memoizes computed results and always continues from the
 * last known value because the sequence is strictly increasing.  For
 * small m (m ≤ 93) a precise comparison is performed using 64‑bit
 * arithmetic; for larger m a long‑double logarithmic comparison is
 * used, guaranteeing correctness while avoiding overflow in the 64‑bit
 * build.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/fib.h"
#include "../utils/fib_u64.h"

static seq_t *cache = NULL;     /* cache[n] = a(n), 1‑based; cache[0] unused */
static idx_t   cache_n = 0;     /* highest n for which cache[n] is valid */

/* exact limit for 64‑bit Fibonacci: F93 fits in 64 bits */
#define FIB_MAX_64 93

/* ---------- logarithmic constants ---------- */
static const long double LN_PHI     = logl((1.0L + sqrtl(5.0L)) / 2.0L);
static const long double LN_SQRT5   = 0.5L * logl(5.0L);

/* ---------- comparison: Fib(m) ≥ m^n ---------- */
static bool fib_ge_pow(idx_t m, idx_t n)
{
    /* --------------- small m (exact) --------------- */
    if (m <= FIB_MAX_64) {
        uint64_t fibm = fib_u64(m);

        /* compute m^n with early exit if it exceeds fibm */
        uint64_t power = 1;
        for (idx_t i = 0; i < n; ++i) {
            if (power > fibm / m)  /* power * m would exceed fibm */
                return false;
            power *= m;
        }
        return true;                /* fibm ≥ m^n */
    }

    /* --------------- large m (logarithmic) --------- */
    long double ln_fib  = (long double)m * LN_PHI - LN_SQRT5;
    long double ln_pow  = (long double)n * logl((long double)m);
    return ln_fib >= ln_pow;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) {                 /* sequence undefined for n = 0 */
        return 0;
    }

    /* lazily grow the cache */
    if (n > cache_n) {
        idx_t new_size = n + 1;   /* 1‑based indexing */
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        }
        if (!cache) return 0;     /* allocation failure propagates 0 */
        /* compute new entries */
        for (idx_t k = cache_n + 1; k <= n; ++k) {
            seq_t m = (k == 1) ? 2 : cache[k - 1];
            while (!fib_ge_pow((idx_t)m, k)) {
                ++m;
            }
            cache[k] = m;
        }
        cache_n = n;
    }

    return cache[n];
}
