/*
 * A223027  G.f.: sqrt( 1 + Sum_{n>=1} 4*x^(n^2) ).
 * https://oeis.org/A223027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; for n>0: a(n) = ( (4 if n is a perfect square else 0) -
 *            Σ_{i=1}^{n-1} a(i)*a(n-i) ) / 2 This generates the coefficients
 *            of sqrt(1 + Σ_{k≥1} 4 x^{k²})
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 19, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/*  External types supplied by the build  */
extern seq_t a(idx_t n);

/* ---------- cache ---------- */
static seq_t *cache = NULL;          /* array of a[0 .. cache_cap-1] */
static size_t cache_cap = 0;          /* capacity of the array          */
static idx_t last_computed = 0;       /* highest index that has been
                                         computed (a[0] is ready after first
                                         allocation)                         */

/*----- integer square root (floor) for idx_t values ----------*/
static inline idx_t
int_sqrt(idx_t n)
{
    if (n == (idx_t)0) return (idx_t)0;
    idx_t low = (idx_t)0;
    idx_t high = n;
    idx_t result = (idx_t)0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid > (idx_t)0 && mid > n / mid) {    /* mid*mid would overflow or be > n */
            high = mid - 1;
        } else {
            idx_t square = mid * mid;              /* safe: square <= n < 2^64 */
            if (square == n) return mid;
            if (square < n) {
                low = mid + 1;
                result = mid;
            } else {
                high = mid - 1;
            }
        }
    }
    return result;
}

/*----- ensure the cache can hold index n (inclusive) ---------*/
static void
ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap) return;

    if (cache == NULL) {
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;                     /* a(0)=1 */
        for (size_t i = 1; i < needed; ++i)
            cache[i] = (seq_t)0;
        cache_cap = needed;
        last_computed = 0;                       /* a[0] already computed */
    } else {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (size_t i = cache_cap; i < needed; ++i)
            cache[i] = (seq_t)0;
        cache_cap = needed;
    }
}

/*----- compute a[k] for k in [start .. end] -------------------*/
static void
compute_terms(idx_t start, idx_t end)
{
    for (idx_t k = start; k <= end; ++k) {
        seq_t sum = (seq_t)0;
        for (idx_t i = 1; i < k; ++i)
            sum += cache[(size_t)i] * cache[(size_t)(k - i)];
        seq_t constant = (seq_t)0;
        idx_t r = int_sqrt(k);
        if (r * r == k)
            constant = (seq_t)4;
        seq_t diff = constant - sum;
        cache[(size_t)k] = diff / (seq_t)2;     /* division is exact */
    }
}

/*--------------------- public function ------------------------*/
seq_t
a(idx_t n)
{
    if (cache == NULL)
        ensure_cache(n);                     /* first call: a[0] set to 1 */

    ensure_cache(n);                         /* expand cache if needed */

    if (n > last_computed) {
        compute_terms(last_computed + 1, n);
        last_computed = n;
    }
    return cache[(size_t)n];
}
