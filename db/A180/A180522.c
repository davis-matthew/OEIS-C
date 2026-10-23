/*
 * A180522  Primes that become a different prime under the mapping 0 <=> 7.
 * https://oeis.org/A180522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = nth prime p such that swapping the digits 0 ↔ 7 yields a
 *            different prime
 * Range:     u8 n <= 5, u16 n <= 420, u32 n <= 526, u64 n <= 1956,
 *            u128 n <= 1956, big any n
 * Helpers:   is_prime_big, map_swap_zero_seven
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function relies on the helper at ../utils/is_prime_big.h
   which provides `bool is_prime_big(seq_t n);` */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/map_swap_zero_seven.h"

seq_t a(idx_t n) {
    /* Static state for memoization */
    static seq_t *seq = NULL;      /* array of found primes */
    static size_t seq_cap = 0;     /* capacity of seq */
    static idx_t  seq_len = 0;     /* number of primes stored */
    static seq_t  cand = (seq_t)0; /* current candidate to test */
    static bool   started = false;

    if (!started) {
        cand = (seq_t)2;
        started = true;
    }

    /* Generate primes until we have at least n terms */
    while (seq_len < n) {
        if (is_prime_big(cand)) {
            seq_t mapped = map_swap_zero_seven(cand);
            if (mapped != cand && is_prime_big(mapped)) {
                /* Grow the array if necessary */
                if (seq_len == seq_cap) {
                    seq_cap = seq_cap ? seq_cap * 2 : 8;
                    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                }
                seq[seq_len++] = cand;
            }
        }
        cand++; /* move to the next integer */
    }

    return seq[n - 1];
}
