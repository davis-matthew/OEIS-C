/*
 * A048521  Primes expressible as the sum of an integer plus its digit sum.
 * https://oeis.org/A048521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 3005, u32 n <= 5456, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes equal to an integer plus its digit sum:
   a(n) = the n-th prime p for which ∃ k  (p = k + digit_sum(k)) */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n) {
    /* static state for memoization */
    static seq_t *found = NULL;       /* array of found primes          */
    static size_t found_size = 0;     /* number of primes stored        */
    static size_t found_cap = 0;      /* capacity of array              */
    static idx_t next_k = 1;          /* next integer to test           */

    /* generate primes until we have at least n terms */
    while (found_size < (size_t)n) {
        seq_t k = (seq_t)next_k;
        seq_t p = k + digit_sum(k);
        if (is_prime_big(p)) {
            /* check whether p already in found */
            bool duplicate = false;
            for (size_t i = 0; i < found_size; ++i) {
                if (found[i] == p) { duplicate = true; break; }
            }
            if (!duplicate) {
                grow_seq(&found, &found_cap, found_size + 1);
                found[found_size++] = p;
            }
        }
        ++next_k;
    }
    return found[n - 1];
}
