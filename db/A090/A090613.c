/*
 * A090613  Numbers k such that the k-th prime is congruent to 3 mod 7.
 * https://oeis.org/A090613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes A090613: numbers k such that the k‑th prime ≡ 3 (mod 7).
 * The function returns the index of the n‑th qualifying prime.
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

seq_t a(idx_t n) {
    /* Static storage for computed results. */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;

    /* State for prime generation. */
    static idx_t candidate = 2;
    static idx_t prime_index = 0;
    static bool initialized = false;

    /* Initialize static data on first call. */
    if (!initialized) {
        seq_cap = 16;
        seq = mem_new(seq_t, seq_cap);
        initialized = true;
    }

    /* Return already known value. */
    if (n <= seq_len) {
        return seq[n - 1];
    }

    /* Generate primes until we have at least n qualifying indices. */
    while (seq_len < (size_t)n) {
        if (is_prime(candidate)) {
            prime_index++;
            if ((candidate % 7) == 3) {
                if (seq_len == seq_cap) {
                    seq_cap *= 2;
                    seq = mem_resize(seq_t, seq, seq_cap);
                }
                seq[seq_len++] = (seq_t)prime_index;
            }
        }
        candidate++;
    }

    return seq[n - 1];
}
