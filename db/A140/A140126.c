/*
 * A140126  Partial sums of A001912.
 * https://oeis.org/A140126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{i=1..n} [4*i*i + 1 is prime] (i.e. the sum of all i ≤
 *            n such that 4*i^2 + 1 is prime)
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"   /* provides bool is_prime_small(uint64_t) */

seq_t a(idx_t n)
{
    /* ---------- static memoisation tables ---------- */
    /* cache[k] = a(k) for k >= 1, cache[0] is unused but keeps indices aligned */
    static seq_t *cache        = NULL;   /* array of seq_t */
    static size_t cache_cap    = 0;      /* allocated capacity (in seq_t elements) */
    static idx_t  max_n        = 0;      /* highest n that has been computed   */
    static idx_t  next_i       = 1;      /* next i to test */
    static seq_t  curr_sum     = 0;      /* running sum of qualifying i's      */

    /* ---------- initialise on first call ---------- */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) return 0;          /* out of memory – caller will fault */
        cache[0] = 0;                         /* unused slot for 0th index */
    }

    /* ---------- if request already satisfied ---------- */
    if (n <= max_n)
        return cache[n];

    /* ---------- grow cache if necessary ---------- */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) return 0;           /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* ---------- compute missing values ---------- */
    for (/* continuation until max_n == n */; max_n < n; ++next_i) {
        /* compute 4*i*i + 1, using 64-bit arithmetic to avoid overflow
           (fits easily for n up to 10 000, value ≤ 400 000 001) */
        uint64_t ii   = (uint64_t)next_i * next_i;
        uint64_t val  = 4ULL * ii + 1ULL;

        if (is_prime_small(val)) {
            ++max_n;
            curr_sum += (seq_t)next_i;
            cache[max_n] = curr_sum;
        }
    }

    return cache[n];
}
