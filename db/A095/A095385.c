/*
 * A095385  Numbers k with property that k is a peak value in 3x+1 trajectory
 *          such that both k+1 and k-1 are prime numbers.
 * https://oeis.org/A095385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   isPrimeGeneric, is_peak
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A095385
   A(n) = smallest even integer k such that
      - k-1 and k+1 are prime (twin primes),
      - k is a peak in its Collatz (3x+1) trajectory
        (i.e., no value in the trajectory exceeds k).
   The function searches k successively, memoising the results. */

/* standard headers */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* primality test from the utilities library */
#include "../utils/isPrimeGeneric.h"
#include "../utils/is_peak.h"

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* static memoisation state */
    static seq_t *cache         = NULL;
    static idx_t cache_len      = 0;
    static idx_t cache_cap      = 0;
    static seq_t next_k         = (seq_t)4;  /* first candidate */
    static idx_t found          = 0;

    /* return cached term if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* enlarge cache if necessary */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp)  /* safe fallback */
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* search until we have found n terms */
    while (found < n) {
        /* twin‑prime test */
        if (isPrimeGeneric(next_k - 1) && isPrimeGeneric(next_k + 1)) {
            /* peak test */
            if (is_peak(next_k))
                cache[found++] = next_k;
        }
        /* advance to next even integer */
        next_k = next_k + 2;
    }
    cache_len = found;
    return cache[n - 1];
}
