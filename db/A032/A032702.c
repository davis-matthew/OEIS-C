/*
 * A032702  Numbers k such that k prefixed by '2' and followed by '1' is
 *          prime.
 * https://oeis.org/A032702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find k such that 2*10^(digits(k)+1)+k*10+1 is prime */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static idx_t cap = 0;
    static idx_t len = 0;
    static idx_t next_k = 1;          /* next k to test */
    static uint64_t pow10 = 10;       /* 10^digits(current k) */

    /* extend the sequence until we have n terms */
    while (len < n) {
        /* bump to next digit length when needed */
        if (next_k >= pow10) {
            pow10 *= 10;              /* pow10 = 10^digits(next_k) */
        }

        /* candidate number: 2*10^(digits(k)+1) + k*10 + 1 */
        uint64_t cand = 20ULL * pow10 + (uint64_t)next_k * 10ULL + 1ULL;

        if (is_prime((idx_t)cand)) {
            /* ensure capacity for the next term */
            if (len == cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                cap = new_cap;
            }
            seq[len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return seq[n - 1];
}
