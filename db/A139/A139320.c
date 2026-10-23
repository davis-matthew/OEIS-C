/*
 * A139320  Values of k in A139319.
 * https://oeis.org/A139320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A139320: a(n) = minimal k such that n*k‑1 is
 * coprime to all earlier terms i*k(i)-1 for 1 ≤ i < n.
 * The function memoises computed values and works for any call
 * order.  It is compatible with pure C (C11) and C++ (C++17)
 * where seq_t may be a simple unsigned type or an arbitrary‑precision
 * integer class. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* static cache of already computed k values (a(1), a(2), …)          */
static seq_t *k_cache = NULL;    /* array of seq_t */
static size_t cache_cap   = 0;   /* allocated capacity (bytes/seq_t)   */
static idx_t  cache_max   = 0;   /* largest n for which a(n) is known  */

/* enlarge the cache so that it can hold at least n entries   */
static void ensure_cache_cap(idx_t n)
{
    if (n <= (idx_t)cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (n > (idx_t)new_cap) new_cap <<= 1;

    /* allocate new array and copy existing elements */
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure */

    for (size_t i = 0; i < cache_cap; ++i)
        tmp[i] = k_cache[i];

    free(k_cache);
    k_cache   = tmp;
    cache_cap = new_cap;
}

/* ---------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* undefined for n < 1 */

    ensure_cache_cap(n);

    /* compute any missing terms up to n */
    for (idx_t m = cache_max + 1; m <= n; ++m) {
        if (m == 1) {
            /* a(1) = 2  (the smallest k giving a positive coprime) */
            k_cache[0] = 2;
        } else {
            seq_t k_candidate = 1;
            bool  found       = false;
            seq_t cand_val;

            while (!found) {
                cand_val = (seq_t)m * k_candidate - 1;
                bool ok  = true;

                for (idx_t i = 1; i < m; ++i) {
                    seq_t prev_val = (seq_t)i * k_cache[i - 1] - 1;
                    if (gcd(cand_val, prev_val) != 1) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    found = true;
                } else {
                    ++k_candidate;
                }
            }
            k_cache[m - 1] = k_candidate;
        }
        cache_max = m;
    }

    return k_cache[n - 1];
}
