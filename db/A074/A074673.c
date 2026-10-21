/*
 * A074673  Four-digit distinct-digit primes.
 * https://oeis.org/A074673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 510, u32 n <= 510, u64 n <= 510, u128 n <= 510,
 *            big any n
 * Helpers:   has_distinct_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the n-th 4‑digit prime with all distinct digits. */
/* A simple sieve is not needed; we search numbers 1000…9999, test for
   primality (using trial division by primes ≤ 97) and distinct digits.
   The first 510 such primes are stored in a static array and reused
   for subsequent calls. */

#include <stdbool.h>
#include "../utils/has_distinct_digits.h"

static void init_sequence(void);

/* List of primes ≤ 100, used for the trial‑division primality test. */
static const unsigned char small_primes[] = {
   2,3,5,7,11,13,17,19,23,29,
   31,37,41,43,47,53,59,61,67,71,
   73,79,83,89,97
};
static const unsigned int small_primes_count = sizeof(small_primes);

/* Static storage for the sequence values. */
static idx_t      seq_count = 0;
static seq_t      seq_values[512];
static bool       seq_initialized = false;

/* Simple primality test for numbers ≤ 9999 using the small prime list. */
static bool is_prime(idx_t x)
{
    if (x < 2) return false;
    if (x == 2) return true;
    if ((x & 1) == 0) return false;  /* even numbers >2 */
    unsigned i;
    for (i = 0; i < small_primes_count; ++i) {
        unsigned p = (unsigned)small_primes[i];
        if ((seq_t)p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Populate seq_values with all 4‑digit primes having distinct digits. */
static void init_sequence(void)
{
    if (seq_initialized) return;
    idx_t candidate;
    for (candidate = 1000; candidate <= 9999; ++candidate) {
        if (has_distinct_digits(candidate) && is_prime(candidate)) {
            seq_values[seq_count++] = (seq_t)candidate;
            if (seq_count >= 512) break;  /* safety; only 510 exist */
        }
    }
    seq_initialized = true;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    init_sequence();
    if (n == 0) return 0;            /* undefined for offset 1 */
    if ((n - 1) < seq_count) {
        return seq_values[n - 1];
    }
    return 0;  /* n beyond known terms */
}
