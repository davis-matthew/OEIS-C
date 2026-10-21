/*
 * A224469  Primes of the form 8^n - n.
 * https://oeis.org/A224469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   is_prime_big, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A224469
 * The sequence lists primes of the form 8^k - k in order of increasing k,
 * starting with k = 1.
 * The function a(idx_t n) returns the n-th prime in this list.
 */

#include <stdlib.h>
#include <stdbool.h>
/* We assume the helper functions are available in the build environment. */
#include "../utils/pow_seq.h"          /* pow_seq(seq_t a, int k) */
#include "../utils/is_prime_big.h"     /* bool is_prime_big(seq_t n) */

seq_t a(idx_t n) {
    /* static state to enable memoisation of previously computed terms */
    static idx_t count = 0;          /* number of primes already found */
    static idx_t k_next = 1;         /* next k to test */
    static seq_t *primes = NULL;     /* dynamic array of found primes */
    static size_t cap = 0;           /* capacity of the primes array */

    /* keep computing until we have at least n primes */
    while (count < n) {
        /* candidate prime: 8^k_next - k_next */
        seq_t candidate = pow_seq((seq_t)8, (int)k_next) - (seq_t)k_next;

        /* test primality */
        if (is_prime_big(candidate)) {
            /* resize array if necessary */
            if (count >= cap) {
                size_t new_cap = cap ? cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0; /* allocation failure – return zero */
                primes = tmp;
                cap = new_cap;
            }
            /* store the prime */
            primes[count++] = candidate;
        }
        k_next++;  /* advance to the next k */
    }

    /* return the n-th prime (1‑based index) */
    return primes[n - 1];
}
