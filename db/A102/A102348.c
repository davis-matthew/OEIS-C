/*
 * A102348  Decimated primes: every 10th prime has been omitted.
 * https://oeis.org/A102348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = nth prime after removing every 10th prime from the
 *            standard prime list
 * Range:     u8 n <= 49, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   estimate_prime_upper_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the nth term of OEIS sequence A102348.
   It uses a simple sieve of Eratosthenes up to an estimated bound
   (obtained from estimate_prime_upper_bound for the needed number
   of full primes).  Primes whose index in the full prime list is a
   multiple of 10 are skipped.  The function works for any
   integer type seq_t (built‑in unsigned or arbitrary‑precision) and
   idx_t (uint64_t).  It is safe under C11 and C++17. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/estimate_prime_upper_bound.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1 – guard against zero */

    /* How many full primes do we need?  We need at least n + floor(n/9) primes,
       i.e. ceil(n*10/9).  Add a small safety margin. */
    idx_t needed_full = n * 10 / 9 + 10;

    /* Upper bound for the needed_full-th prime. */
    idx_t limit = estimate_prime_upper_bound(needed_full);

    /* Allocate sieve array.  Casting to (uint8_t*) makes the code valid C++17. */
    uint8_t *is_prime = (uint8_t *)malloc((size_t)limit + 1);
    if (!is_prime) return (seq_t)0;  /* memory failure is unlikely for the test ranges */

    /* Initialize sieve: mark all numbers as potential primes. */
    memset(is_prime, 1, (size_t)limit + 1);
    is_prime[0] = 0;
    is_prime[1] = 0;

    /* Eratosthenes sieve. */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (!is_prime[p]) continue;
        for (idx_t j = p * p; j <= limit; j += p) {
            is_prime[j] = 0;
        }
    }

    /* Count primes, skipping every 10th one in the full prime sequence. */
    idx_t full_index = 0;      /* 1‑based index in the full prime list */
    idx_t retained_index = 0;  /* number of primes kept so far */
    for (idx_t p = 2; p <= limit; ++p) {
        if (!is_prime[p]) continue;
        ++full_index;
        if ((full_index % 10) != 0) {
            ++retained_index;
            if (retained_index == n) {
                free(is_prime);
                return (seq_t)p;
            }
        }
    }

    /* If we reached here, the estimate was insufficient (should not happen). */
    free(is_prime);
    return (seq_t)0;
}
