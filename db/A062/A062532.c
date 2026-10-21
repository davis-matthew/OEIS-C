/*
 * A062532  Odd nonprimes squared, or A014076(n)^2.
 * https://oeis.org/A062532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = (odd non‑prime number having index n)^2 (odd non‑primes
 *            are 1,9,15,25,27,… – i.e. odd composite numbers, counted from n
 *            = 1)
 * Range:     u8 n <= 3, u16 n <= 75, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_LIMIT 500000   /* pre‑computed maximum odd limit */

/* Prime sieve and odd–composite list */
static uint8_t is_prime[MAX_LIMIT + 1];
static uint64_t odd_composites[MAX_LIMIT / 2];
static size_t composite_cnt = 0;
static bool initialized = false;

static void init_sequence(void)
{
    size_t i, j;
    /* 0 and 1 are not prime */
    for (i = 0; i <= MAX_LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    /* Sieve of Eratosthenes */
    for (i = 2; i * i <= MAX_LIMIT; ++i) {
        if (is_prime[i]) {
            for (j = i * i; j <= MAX_LIMIT; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    /* Collect odd composites (odd non‑primes) */
    composite_cnt = 0;
    for (i = 1; i <= MAX_LIMIT; i += 2) {
        if (i == 1 || !is_prime[i]) {
            odd_composites[composite_cnt++] = i;
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_sequence();

    /* Return 0 for indices below the offset (not part of the spec) */
    if (n < 1 || (size_t)n > composite_cnt) return 0;

    seq_t value = (seq_t)odd_composites[n - 1];
    return value * value;
}
