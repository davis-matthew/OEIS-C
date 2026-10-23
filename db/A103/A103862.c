/*
 * A103862  Numbers n such that the string 10000n is the decimal expansion of
 *          a prime number.
 * https://oeis.org/A103862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   is_prime_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A103862
 *  a(n) = the n-th integer m such that the decimal number 10000·10^d + m
 *  (where d = number of decimal digits of m) is prime.
 *  Implementation uses a simple incremental sieve: iterate over all m,
 *  test 10000·10^d + m for primality with deterministic Miller–Rabin
 *  (is_prime_ull from the build utilities).  Carmichael tests are
 *  fast enough for the required range (m up to a few million).
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_ull.h"

/* Helper: number of decimal digits in x (x>=1) */
static inline idx_t num_digits(idx_t x) {
    idx_t d = 0;
    do {
        ++d;
        x /= 10;
    } while (x);
    return d;
}

/* Pre‑computed powers of 10 up to 10^18 */
static const uint64_t pow10_table[20] = {
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
    1000000000000000000ULL
};

/* Sequence storage: dynamically grow as needed. */
static seq_t *seen = NULL;
static idx_t seen_cap = 0;
static idx_t seen_size = 0;
static idx_t next_candidate = 1;   /* next m to test */

/* Ensure capacity in the seen array. */
static void ensure_capacity(idx_t needed) {
    if (seen_size >= needed)
        return;
    if (seen_cap == 0)
        seen_cap = 16;          /* initial allocation */
    while (seen_cap < needed)
        seen_cap <<= 1;          /* double until enough */
    if (!seen)
        seen = (seq_t*)malloc(seen_cap * sizeof(seq_t));
    else
        seen = (seq_t*)realloc(seen, seen_cap * sizeof(seq_t));
}

/* Main sequence generator */
seq_t a(idx_t n) {
    if (n == 0) { /* Offset is 1, but callers may ask for n=0; return 0 */
        return (seq_t)0;
    }

    /* Bring the list up to at least n items */
    ensure_capacity(n);

    while (seen_size < n) {
        /* Skip even numbers and multiples of 5 – 10000*10^d is even and
         * divisible by 5, thus the only chance for primeness is when m is
         * odd and not a multiple of 5. */
        if ((next_candidate & 1ULL) && (next_candidate % 5ULL)) {
            idx_t d = num_digits(next_candidate);
            uint64_t val = 10000ULL * pow10_table[d] + next_candidate;
            if (is_prime_ull(val)) {
                seen[seen_size++] = (seq_t)next_candidate;
            }
        }
        ++next_candidate;
    }

    return seen[n-1];
}
