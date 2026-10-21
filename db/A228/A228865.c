/*
 * A228865  Primes of the form n^2 + pi(n).
 * https://oeis.org/A228865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 32, u32 n <= 37, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A228865 – primes of the form n^2 + pi(n)      (index 1-based) */

#include <stdbool.h>
#include "../utils/is_prime.h"        /* bool is_prime(idx_t x);   */
#include "../utils/is_prime_big.h"    /* bool is_prime_big(seq_t n); */

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n < 1, return 0 to be safe */

    idx_t count = 0;        /* how many primes of the form have been found */
    idx_t k = 1;            /* current n being tested */
    idx_t pi_cnt = 0;       /* pi(k) for the current k */

    for (;;) {
        ++k;                       /* move to next n */
        if (is_prime(k)) ++pi_cnt; /* update pi(k) */
        seq_t cand = (seq_t)k * k + (seq_t)pi_cnt; /* n^2 + pi(n) */
        if (is_prime_big(cand)) {
            ++count;
            if (count == n) return cand; /* nth term found */
        }
    }
}
