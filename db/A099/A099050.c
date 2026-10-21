/*
 * A099050  a(1)=1; for n>=2, a(n)=sum(1<=i<=j<=n-1, gcd(a(i),a(j))).
 * https://oeis.org/A099050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS sequence A099050
 * Formula: a(n) = sum_{1 <= i <= j <= n-1} gcd(a(i), a(j))
 * Initial values: a(1) = 1, a(2) = 1
 *
 * The implementation builds the sequence incrementally.
 * For each new index k we already know g(k-1) =
 *   sum of gcds over all unordered pairs among a(1)…a(k-1).
 * Then a(k) = g(k-1) and g(k) = g(k-1) + sum_{i=1}^{k} gcd(a(i), a(k)).
 * This requires O(k) gcd computations for the k‑th term,
 * yielding a total of O(n²) gcds for all terms up to n.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;   /* maximum index that has been filled */
    static seq_t current_g = 0;    /* g(max_cached) = sum_{i<=j<=max_cached} gcd(a(i),a(j)) */

    /* Undefined behavior for n == 0 is acceptable */
    if (n == 0)
        return (seq_t)0;

    /* Expand cache if necessary */
    if (n > max_cached) {
        size_t new_cap = (size_t)(n + 1);      /* indices 0..n, 0 unused */
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – terminate */
            exit(1);
        }
        cache = new_cache;

        /* Compute new terms */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            if (i == 1) {
                /* Base case */
                cache[1] = (seq_t)1;
                current_g = gcd(cache[1], cache[1]); /* g(1) = 1 */
            } else {
                /* a(i) = g(i-1) */
                seq_t val = current_g;
                cache[i] = val;

                /* Compute sum of gcd(a(j), val) for j=1..i */
                seq_t sum_gcd = (seq_t)0;
                for (idx_t j = 1; j <= i; ++j) {
                    sum_gcd += gcd(cache[j], val);
                }
                /* Update cumulative sum g(i) */
                current_g += sum_gcd;
            }
        }
        max_cached = n;
    }

    return cache[n];
}
