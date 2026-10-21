/*
 * A062020  a(n) = Sum_{i=1..n} Sum_{j=1..i} (prime(i) - prime(j)).
 * https://oeis.org/A062020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} Sum_{j=1..i} (prime(i)-prime(j))
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0, a(n)=a(n-1)+n*prime(n)-Sum_{i=1..n}prime(i) */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    /* a(1) is defined; for n < 1 return 0 (offset not used) */
    if (n < 1) return 0;

    /* Get first n primes */
    uint32_t *primes = NULL;
    size_t   pc = 0;
    sieve_first_n_primes((idx_t)n, &primes, &pc);   /* pc == n */

    seq_t pref = 0;   /* sum of primes up to current i */
    seq_t a_val = 0;  /* a(n) being built */

    for (idx_t i = 1; i <= n; ++i) {
        pref += (seq_t)primes[i-1];
        a_val += (idx_t)i * (seq_t)primes[i-1] - pref;
    }

    free(primes);
    return a_val;
}
