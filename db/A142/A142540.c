/*
 * A142540  Primes congruent to 10 mod 53.
 * https://oeis.org/A142540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime ≡ 10 (mod 53)
 * Range:     u8 none, u16 n <= 118, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static idx_t *wanted = NULL;
static size_t wanted_cap = 0;
static idx_t wanted_cnt = 0;
static idx_t next_candidate = 2;

/* Ensure that the wanted array can store another prime. */
static void ensure_wanted_cap(void) {
    if (wanted_cnt + 1 > wanted_cap) {
        size_t new_cap = wanted_cap ? (wanted_cap << 1) : 32;
        idx_t *tmp = (idx_t*)realloc(wanted, new_cap * sizeof(idx_t));
        if (!tmp) return;   /* allocation failure is ignored */
        wanted = tmp;
        wanted_cap = new_cap;
    }
}

/* Append a prime that satisfies the congruence to the array. */
static void add_wanted(idx_t prime) {
    ensure_wanted_cap();
    if (!wanted) return;
    wanted[wanted_cnt++] = prime;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for n=0 */

    while (wanted_cnt < n) {
        idx_t candidate = next_candidate;

        if (candidate == 2) {          /* handle 2 first */
            if (is_prime(candidate) && (candidate % 53u) == 10u)
                add_wanted(candidate);
            next_candidate = 3;
            continue;
        }

        if (is_prime(candidate)) {
            if ((candidate % 53u) == 10u)
                add_wanted(candidate);
        }

        next_candidate = candidate + 2;   /* stay odd after 2 */
    }

    return (seq_t)wanted[n - 1];
}
