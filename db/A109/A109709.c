/*
 * A109709  Triangle in which n-th row gives the prime factors of the n-th
 *          composite number with repetition.
 * https://oeis.org/A109709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   is_prime, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A109709: a(n) is the n-th prime factor (with multiplicity) of
   successive composite numbers, flattened into a single sequence.
   The function generates composites in order, factors them,
   and appends each prime factor to an internal buffer.
   The buffer is cached so that calls return immediately after the
   required amount has been computed, regardless of the order of calls. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/push_seq.h"

static seq_t  *flat          = NULL;
static idx_t   flat_cap      = 0;
static idx_t   flat_size     = 0;
static uint64_t next_composite = 4;

/* Returns the n‑th element of the flattened prime‑factor triangle. */
seq_t a(idx_t n)
{
    while (flat_size < n) {
        /* Find the next composite number. */
        uint64_t c;
        do {
            c = next_composite++;
        } while (is_prime(c));

        /* Factorise c and record each prime factor. */
        uint64_t rem = c;
        for (uint64_t d = 2; d * d <= rem; ++d) {
            while (rem % d == 0) {
                push_seq(&flat, &flat_cap, &flat_size, (seq_t)d);
                rem /= d;
            }
        }
        if (rem > 1) push_seq(&flat, &flat_cap, &flat_size, (seq_t)rem);
    }

    return flat[n - 1];
}
