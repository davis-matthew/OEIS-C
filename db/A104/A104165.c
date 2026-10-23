/*
 * A104165  Sophie Germain type primes where 7*Prime[n]=2*Prime[m]+1.
 * https://oeis.org/A104165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (7*Prime[n]-1)/2 where Prime[n] is the n-th prime and the
 *            result must also be prime. The sequence thus consists of all
 *            primes of the form (7*p-1)/2 with p prime and (7*p-1)/2 also
 *            prime. Implementation uses a single global sieve up to a fixed
 *            limit and generates the terms lazily
 * Range:     u8 n <= 3, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static const uint64_t SIEVE_LIMIT = 4000000ULL;   /* enough for n≈10 000 */
static uint8_t *is_comp = NULL;                  /* composite flags */
static uint64_t *prime_list = NULL;              /* list of primes up to SIEVE_LIMIT */
static size_t primes_count = 0;                  /* number of primes generated */
static seq_t *a_values = NULL;                   /* computed sequence terms */
static size_t a_count = 0;                       /* number of terms stored */
static size_t prime_idx = 0;                     /* index into prime_list */

/* initialise the global sieve and prime list */
static void init_sieve(void) {
    size_t i, j, limit = (size_t)SIEVE_LIMIT;
    /* Allocate and clear composite flag array */
    is_comp = (uint8_t*)calloc(limit + 1, 1);   /* 0 ⇒ prime, 1 ⇒ composite */
    if (!is_comp) exit(EXIT_FAILURE);
    is_comp[0] = is_comp[1] = 1;

    /* Eratosthenes sieve */
    for (i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (j = i * i; j <= limit; j += i) {
                is_comp[j] = 1;
            }
        }
    }

    /* Reserve space for prime list (approximate) */
    size_t est = limit / 10 + 10;               /* ~= number of primes up to limit */
    prime_list = (uint64_t*)malloc(est * sizeof(uint64_t));
    if (!prime_list) exit(EXIT_FAILURE);
    primes_count = 0;

    /* Fill prime list */
    for (i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (primes_count == est) {           /* grow if needed */
                est *= 2;
                prime_list = (uint64_t*)realloc(prime_list, est * sizeof(uint64_t));
                if (!prime_list) exit(EXIT_FAILURE);
            }
            prime_list[primes_count++] = i;
        }
    }
}

/* compute a_values up to at least n terms */
static void compute_a_up_to(size_t n) {
    if (a_count >= n) return;                 /* already have enough */

    while (a_count < n) {
        if (prime_idx >= primes_count) break;  /* should not happen for n≤10 000 */

        uint64_t p = prime_list[prime_idx++];
        uint64_t cand = (p * 7ULL - 1ULL) / 2ULL;   /* candidate value */

        if (cand <= SIEVE_LIMIT && !is_comp[(size_t)cand]) {
            seq_t *tmp = (seq_t*)realloc(a_values, (a_count + 1) * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            a_values = tmp;
            a_values[a_count++] = (seq_t)cand;
        }
    }
}

seq_t a(idx_t n) {
    /* Lazy initialisation */
    static int initialized = 0;
    if (!initialized) {
        init_sieve();
        initialized = 1;
    }

    if (n == 0) return (seq_t)0;             /* behaviour below offset irrelevant */

    compute_a_up_to((size_t)n);
    return a_values[(size_t)n - 1];           /* offset 1 → index 0 */
}
