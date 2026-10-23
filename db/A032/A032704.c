/*
 * A032704  Numbers k such that k prefixed by '4' and followed by '1' is
 *          prime.
 * https://oeis.org/A032704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 269, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_count, ensure_capacity, isPrimeGeneric, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for k such that the integer 4k1 is prime.
   Returns the n-th such k, with a(1)=0. */
/* Build by iterating k and testing primality of 4*10^(digits(k)+1)+k*10+1
   Caching results so that non‑sequential calls are efficient. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/pow10.h"
#include "../utils/digit_count.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n) {
    /* static cache: all found k values */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated capacity */
    static size_t used = 0;         /* number of k's stored */
    static seq_t next_k = 0;        /* next candidate k to test */

    /* Grow cache until we have enough entries */
    while (used < n) {
        /* count digits of next_k (uint64_t is enough for required n) */
        int d = digit_count((uint64_t)next_k);
        /* construct N = 4*10^(d+1) + next_k*10 + 1 */
        seq_t pow = pow10((idx_t)(d + 1));
        seq_t N = (seq_t)4 * pow + (seq_t)next_k * (seq_t)10 + (seq_t)1;
        /* test primality */
        if (isPrimeGeneric(N)) {
            /* ensure capacity for next element */
            if (used + 1 > cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                ensure_capacity(new_cap, &cap, (void **)&cache, sizeof(seq_t));
            }
            cache[used++] = next_k;
        }
        next_k++;
    }
    return cache[n - 1];
}
