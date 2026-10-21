/*
 * A061461  Primes which produce primes when the leading digit is moved to the
 *          end.
 * https://oeis.org/A061461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 979, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrime, push_idx, rotate_first_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A061461: primes p such that moving the leading decimal digit to the end
   yields another prime. This implementation builds primes up to an
   increasing limit until it has found enough special primes. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/isPrime.h"   /* provides bool isPrime(idx_t) */
#include "../utils/push_idx.h"
#include "../utils/rotate_first_digit.h"

/* rebuild the prime list and the list of special primes up to 'limit' */
static void rebuild(idx_t limit,
                    idx_t **primes, size_t *p_len, size_t *p_cap,
                    idx_t **special, size_t *s_len, size_t *s_cap) {
    /* free old tables */
    if (*primes) { free(*primes); *primes = NULL; *p_len = 0; *p_cap = 0; }
    if (*special) { free(*special); *special = NULL; *s_len = 0; *s_cap = 0; }

    /* simple sieve of Eratosthenes (limit is guaranteed to fit in memory) */
    size_t sz = (size_t)limit + 1;
    uint8_t *comp = (uint8_t*)calloc(sz, 1);
    for (idx_t i = 2; i <= limit; ++i) {
        if (!comp[(size_t)i]) {
            push_idx(primes, p_len, p_cap, i);
            if (i * i <= limit)
                for (idx_t j = i * i; j <= limit; j += i) comp[(size_t)j] = 1;
        }
    }
    free(comp);

    /* generate the special primes */
    for (size_t i = 0; i < *p_len; ++i) {
        idx_t p = (*primes)[i];
        idx_t r = rotate_first_digit(p);
        if (isPrime(r))
            push_idx(special, s_len, s_cap, p);
    }
}

/* the main sequence function */
seq_t a(idx_t n) {
    static idx_t *primes   = NULL, *special = NULL;
    static size_t p_len = 0, p_cap = 0;
    static size_t s_len = 0, s_cap = 0;
    static idx_t limit = 100000;   /* initial sieve limit */

    if (n == 0) return 0;           /* behaviour below offset is irrelevant */

    /* grow the table until we have at least n terms */
    while (s_len < n) {
        rebuild(limit, &primes, &p_len, &p_cap, &special, &s_len, &s_cap);
        if (s_len < n) {            /* still not enough -> expand limit */
            limit *= 2;
        }
    }
    return (seq_t)special[n - 1];
}
