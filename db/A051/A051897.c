/*
 * A051897  Values of n such that 90n+11, 90n+13, 90n+17, 90n+19 are all
 *          primes.
 * https://oeis.org/A051897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A051897: n such that 90n+11, 90n+13, 90n+17, 90n+19 are all primes.
   The function builds the sequence on demand and caches previously found
   values using seq_alloc / seq_realloc, which are compatible with both
   ordinary integral and arbitrary‑precision seq_t types. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache of sequence values */
    static seq_t *cache = NULL;
    static idx_t count   = 0;   /* number of stored terms   */
    static idx_t capacity = 0;  /* allocated cache size     */
    static idx_t next_candidate = 0; /* next n to test */

    /* offset is 1; behavior for n<1 is not required */
    if (n == 0) return (seq_t)0;

    /* ensure we have at least n terms cached */
    if (count < n) {
        /* grow the cache array if needed */
        if (capacity < n) {
            size_t new_cap = capacity ? capacity * 2 : 4;
            if (new_cap < n) new_cap = n;
            seq_t *tmp;
            if (capacity == 0) {
                tmp = seq_alloc(new_cap);
            } else {
                tmp = seq_realloc(cache, capacity, new_cap);
            }
            cache = tmp;
            capacity = new_cap;
        }

        /* extend the search until we have n terms */
        while (count < n) {
            idx_t a1 = 90 * next_candidate + 11;
            idx_t a2 = 90 * next_candidate + 13;
            idx_t a3 = 90 * next_candidate + 17;
            idx_t a4 = 90 * next_candidate + 19;
            if (is_prime(a1) && is_prime(a2) && is_prime(a3) && is_prime(a4)) {
                cache[count++] = (seq_t)next_candidate;
            }
            ++next_candidate;
        }
    }

    return cache[n - 1];
}
