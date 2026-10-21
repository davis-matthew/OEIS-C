/*
 * A081243  a(n) = Mod[n+(Mod[Prime[n],3]-1),10].
 * https://oeis.org/A081243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Mod[n + (Prime[n] Mod 3 - 1), 10]
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>

/* Helper: estimate an upper bound for the nth prime (n ≥ 1) */
static size_t estimate_upper(idx_t n) {
    if (n < 6) return 15;               /* for tiny n */
    double dn = (double)n;
    double est = dn * (log(dn) + log(log(dn)));
    return (size_t)(est) + 3;           /* safety margin */
}

/* Static storage for the prime list */
static idx_t *primes = NULL;   /* array of primes */
static size_t primes_cnt = 0;  /* number of primes stored */

/* Ensure that the first n primes are available */
static void ensure_primes(idx_t n) {
    if (n <= primes_cnt) return;           /* already have enough */

    /* Compute new sieve limit */
    size_t limit = estimate_upper(n);

    /* Allocate sieve */
    unsigned char *sieve = (unsigned char*)malloc(limit + 1);
    if (!sieve) return; /* out of memory → caller will fail gracefully */

    /* Initialize sieve: 1 = prime candidate, 0 = composite */
    for (size_t i = 0; i <= limit; ++i) sieve[i] = 1;
    if (limit >= 0) sieve[0] = 0;
    if (limit >= 1) sieve[1] = 0;

    /* Sieve of Eratosthenes */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= limit; j += i) {
                sieve[j] = 0;
            }
        }
    }

    /* Allocate new primes array */
    idx_t *new_primes = (idx_t*)malloc((limit + 1) * sizeof(idx_t));
    if (!new_primes) {
        free(sieve);
        return; /* out of memory */
    }

    /* Fill the primes array */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (sieve[i]) {
            if (cnt < limit + 1) {                      /* safety */
                new_primes[cnt++] = (idx_t)i;
            }
        }
    }

    /* Replace old data */
    free(sieve);
    free(primes);
    primes = new_primes;
    primes_cnt = cnt;
}

/* Sequence A081243 */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;     /* undefined for n = 0, return 0 */

    ensure_primes(n);

    /* nth prime (1‑indexed) */
    idx_t p = primes[n - 1];

    int diff = (int)(p % 3) - 1;        /* -1, 0 or 1 */
    int n_mod10 = (int)(n % 10);

    int res = (n_mod10 + diff) % 10;    /* may be negative */
    if (res < 0) res += 10;

    return (seq_t)res;
}
