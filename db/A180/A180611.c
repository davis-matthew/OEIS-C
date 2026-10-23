/*
 * A180611  Largest palindromic prime divisor of semiprime(n). Zero if no such
 *          divisor exists.
 * https://oeis.org/A180611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = largest palindromic prime divisor of the n‑th semiprime
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   _smallest_prime_factor, is_palindrome, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (the n‑th semiprime is the product of two primes, not necessarily distinct) */

#include <stdint.h>
#include <stdlib.h>

#include "../utils/_smallest_prime_factor.h"
#include "../utils/is_prime.h"
#include "../utils/is_palindrome.h"

/* Cache of semiprimes as uint64_t.  The array holds at least the
   first `semiprime_count` semiprimes. */
static uint64_t *semiprime_cache = NULL;
static size_t semiprime_count = 0;
static size_t semiprime_capacity = 0;
/* Next integer to test for being a semiprime. */
static uint64_t next_candidate = 4;

/* Ensure that `semiprime_cache` contains at least `need` elements. */
static void ensure_semiprimes(size_t need)
{
    if (semiprime_count >= need)
        return;

    while (semiprime_count < need) {
        uint64_t p = _smallest_prime_factor(next_candidate);
        if (p != next_candidate) {
            uint64_t q = next_candidate / p;
            if (is_prime(q)) {
                if (semiprime_count == semiprime_capacity) {
                    size_t new_cap = (semiprime_capacity == 0) ? 128 : semiprime_capacity << 1;
                    uint64_t *tmp = (uint64_t *)realloc(semiprime_cache,
                                                         new_cap * sizeof(uint64_t));
                    if (!tmp) /* out of memory – let the caller handle it */
                        return;
                    semiprime_cache = tmp;
                    semiprime_capacity = new_cap;
                }
                semiprime_cache[semiprime_count++] = next_candidate;
            }
        }
        ++next_candidate;
    }
}

/* Main function required by the test harness: */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is 1‑based */

    ensure_semiprimes((size_t)n);

    uint64_t semi = semiprime_cache[(size_t)n - 1];

    seq_t best = 0;
    uint64_t p = _smallest_prime_factor(semi);
    uint64_t q = semi / p;

    if (is_palindrome((seq_t)p, 10))
        best = (seq_t)p;
    if (is_palindrome((seq_t)q, 10) && ((seq_t)q > best))
        best = (seq_t)q;

    return best;
}
