/*
 * A156287  Numbers k such that 4*k-5 is a prime number.
 * https://oeis.org/A156287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = kth integer k such that 4*k-5 is prime
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq, grow_seq, isPrimeGeneric, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/alloc_seq.h"
#include "../utils/grow_seq.h"
#include "../utils/isPrimeGeneric.h"
#include "../utils/mul_small.h"

static seq_t *seq_cache = NULL;
static size_t seq_cache_len = 0;
static size_t seq_cache_cap = 0;
static seq_t seq_last_k = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0; /* safety for 0-index calls */

    if (seq_cache_len >= (size_t)n) {
        return seq_cache[n - 1];
    }

    if (!seq_cache) {
        seq_cache_cap = 16;
        seq_cache = alloc_seq(seq_cache_cap);
    }

    while (seq_cache_len < (size_t)n) {
        seq_last_k++;  /* next integer to test */

        if (seq_last_k >= 2) {          /* 4*k-5 must be ≥3 for a prime candidate */
            seq_t check = mul_small(seq_last_k, 4);
            check -= 5;
            if (isPrimeGeneric(check)) {
                if (seq_cache_len == seq_cache_cap) {
                    grow_seq(&seq_cache, &seq_cache_cap, seq_cache_len + 1);
                }
                seq_cache[seq_cache_len++] = seq_last_k;
            }
        }
    }

    return seq_cache[n - 1];
}
