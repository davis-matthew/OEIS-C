/*
 * A078500  Primes occurring only twice in a decade.
 * https://oeis.org/A078500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 2700, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by generating primes and selecting those that occur exactly twice in their decade. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- Parameters ---------- */
#define LIMIT 20000000U          /* maximum number to sieve   */
#define MAX_PRIMES 200000        /* capacity of prime array   */
#define SEQ_CAP 20000            /* capacity of the result array */

/* ---------- Global state ---------- */
static unsigned char *is_composite = NULL;   /* sieve array */
static uint64_t primes[MAX_PRIMES];          /* list of primes */
static size_t prime_count = 0;              /* number of primes found */

static bool sieve_done = false;             /* flag */

static seq_t seq[SEQ_CAP];                  /* sequence values */
static size_t seq_len = 0;                  /* number of values produced */

static size_t prime_index = 0;              /* next prime to process */
static uint64_t current_decade_start = 0;   /* start of current decade
                                               (multiple of 10) */
static int decade_prime_count = 0;          /* number of primes seen in this decade */
static seq_t decade_primes[2];               /* up to two primes in a decade */

/* ---------- Helper: sieve primes up to LIMIT ---------- */
static void init_sieve(void)
{
    if (sieve_done) return;
    sieve_done = true;

    is_composite = (unsigned char*)malloc((LIMIT + 1) * sizeof(unsigned char));
    if (!is_composite) abort();
    for (size_t i = 0; i <= LIMIT; ++i) is_composite[i] = 0;

    is_composite[0] = is_composite[1] = 1;
    for (uint64_t i = 2; i * i <= LIMIT; ++i) {
        if (!is_composite[i]) {
            for (uint64_t j = i * i; j <= LIMIT; j += i) {
                is_composite[j] = 1;
            }
        }
    }
    for (uint64_t i = 2; i <= LIMIT; ++i) {
        if (!is_composite[i]) {
            if (prime_count < MAX_PRIMES) {
                primes[prime_count++] = i;
            }
        }
    }
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, but avoid underflow */

    /* initialise if necessary */
    init_sieve();

    /* generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        if (prime_index >= prime_count) {
            /* Not enough primes for requested n (should not happen for n<=10000) */
            break;
        }
        uint64_t p = primes[prime_index++];
        uint64_t decade = (p / 10) * 10;

        if (decade != current_decade_start) {
            /* new decade – finalise previous one */
            if (decade_prime_count == 2) {
                seq[seq_len++] = decade_primes[0];
                seq[seq_len++] = decade_primes[1];
            }
            /* reset for the new decade */
            current_decade_start = decade;
            decade_prime_count = 0;
            decade_primes[0] = (seq_t)p;
            decade_prime_count = 1;
        } else {
            if (decade_prime_count < 2) {
                decade_primes[decade_prime_count] = (seq_t)p;
            }
            /* only count beyond two to detect >2 primes */
            decade_prime_count++;
        }
    }

    /* If we stopped because the sieve ran out of primes, the missing terms
       are not reachable for the expected input range. */

    /* Return the requested term (1‑indexed). */
    return seq[(size_t)n - 1];
}
