/*
 * A098116  a(n) = 3^(p-1) + (3^p - 1) where p is the n-th prime.
 * https://oeis.org/A098116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*3^(p-1) - 1 where p is the n-th prime
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   pow_int_seq, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"
#include "../utils/pow_int_seq.h"

seq_t a(idx_t n) {
    /* Undefined behavior for n < 1 (offset is 1). */
    if (n < 1) return 0;

    /* Static state to keep primes and current power of 3. */
    static idx_t *prime_list = NULL;   /* array of primes (1‑based index). */
    static size_t prime_cnt = 0;        /* number of primes stored. */
    static seq_t last_pow3 = (seq_t)1;  /* 3^(last_prime - 1), with last_prime=1 when none. */
    static idx_t last_computed = 0;     /* number of primes processed (>=0). */

    /* Bring prime list up to n if needed. */
    if (n > last_computed) {
        /* Allocate new list of the first n primes. */
        uint32_t *primes32;
        size_t cnt;
        if (prime_list) free(prime_list);
        sieve_first_n_primes(n, &primes32, &cnt);  /* cnt == n */
        prime_list = (idx_t *)malloc(n * sizeof(idx_t));
        for (size_t i = 0; i < n; ++i) prime_list[i] = (idx_t)primes32[i];
        free(primes32);
        prime_cnt = (size_t)n;

        /* Update last_pow3 from last_computed up to the new n. */
        idx_t prev_prime = (last_computed == 0) ? (idx_t)1 : prime_list[last_computed - 1];
        for (size_t i = last_computed; i < n; ++i) {
            idx_t delta = prime_list[i] - prev_prime;
            while (delta-- > 0) last_pow3 *= 3;     /* multiply by 3 delta times */
            prev_prime = prime_list[i];
        }
        last_computed = (idx_t)n;
    }

    /* Compute the requested term. */
    seq_t term;
    if (n == last_computed) {
        /* We already have 3^(p-1) for this n. */
        term = last_pow3;
    } else {
        /* n <= last_computed but not the last one; recompute 3^(p-1) from scratch. */
        idx_t p = prime_list[n - 1];
        term = pow_int_seq((seq_t)3, p - 1);
    }

    term *= 4;   /* 4 * 3^(p-1) */
    term -= 1;   /* minus 1 */
    return term;
}
