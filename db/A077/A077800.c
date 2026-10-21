/*
 * A077800  List of twin primes {p, p+2}.
 * https://oeis.org/A077800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A077800: list of twin primes
   {p, p+2}. The values are obtained by sieving primes up to
   a fixed limit that guarantees at least 10000 terms,
   then extracting twin‑prime pairs and storing each of
   the two members in order.  All data is built only once,
   so repeated calls are O(1).  The implementation is pure
   C (and also compiles with C++17). */
#include <stddef.h>
#include <stdint.h>

#define SIEVE_LIMIT 1000000          /* > 1 000 000 gives > 16 000 terms */
#define MAX_TERMS   20000            /* storage for up to 20 000 terms */

/* Static storage for the sequence */
static seq_t  twin_terms[MAX_TERMS];
static size_t twin_count = 0;

/* Companion array to mark composites */
static uint8_t sieve[SIEVE_LIMIT + 1] = {0};

/* Build the twin‑prime sequence once */
static void build_sequence(void)
{
    static int built = 0;
    if (built) return;
    built = 1;

    /* Eratosthenes sieve */
    size_t i, j;
    sieve[0] = sieve[1] = 1;
    for (i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (!sieve[i]) {
            for (j = i * i; j <= SIEVE_LIMIT; j += i) sieve[j] = 1;
        }
    }

    /* Extract twin primes and fill twin_terms */
    size_t prev_prime = 0;
    for (i = 2; i <= SIEVE_LIMIT && twin_count < MAX_TERMS; ++i) {
        if (!sieve[i]) {
            if (prev_prime && i - prev_prime == 2) {
                twin_terms[twin_count++] = (seq_t)prev_prime;
                if (twin_count < MAX_TERMS) twin_terms[twin_count++] = (seq_t)i;
            }
            prev_prime = i;
        }
    }
}

/* The required sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */
    build_sequence();                    /* ensure data is ready */

    if ((idx_t)n <= twin_count) {
        return twin_terms[(size_t)(n - 1)];
    }
    /* n beyond precomputed range: return 0 (outside defined domain) */
    return (seq_t)0;
}
