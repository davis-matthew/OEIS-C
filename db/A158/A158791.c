/*
 * A158791  Numbers n such that 30*n + 23 is prime.
 * https://oeis.org/A158791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:   numbers n for which (30*n + 23) is prime.
   Function a(idx_t n) returns the n-th such n (a(1)=0, a(2)=1, …). */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n) {
    /* static cache of the sequence */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t count = 0;      /* number of terms cached */
    static idx_t next_k = 0;     /* next candidate index k to test */

    if (n == 0) { /* undefined for 0, but return 0 */
        return (seq_t)0;
    }

    /* extend cache until we have at least n terms */
    while (count < n) {
        seq_t cand = (seq_t)30 * (seq_t)next_k + (seq_t)23;
        if (is_prime_big(cand)) {
            /* grow cache if needed */
            if (count == cap) {
                ensure_capacity(count + 1, &cap, (void **)&cache, sizeof(seq_t));
            }
            cache[count] = next_k;   /* store the n satisfying 30*n+23 prime */
            count++;
        }
        next_k++;  /* move to next k (potential n) */
    }

    return cache[n - 1];
}
