/*
 * A158072  Primes p such that the sum of digits of p - the first digit of p
 *          is prime.
 * https://oeis.org/A158072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   digit_sum, is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime p such that the sum of the digits of p
   minus its first digit is a prime.  No simple closed form is known,
   so primes are generated on demand and the qualifying ones are cached. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

#include "../utils/is_prime.h"      /* is_prime(idx_t)          */
#include "../utils/is_prime_big.h"  /* is_prime_big(seq_t)      */
#include "../utils/digit_sum.h"    /* digit_sum(seq_t)        */

/* Cache of qualifying primes */
static seq_t *qual_primes = NULL;
static size_t qual_len = 0, qual_cap = 0;
static bool initialized = false;
static idx_t next_candidate = 2;

/* Ensure at least 'n' qualifying primes are available in qual_primes */
static void ensure_qualifiers(idx_t n)
{
    if (!initialized) {
        initialized = true;
        qual_cap = 1024;
        qual_primes = (seq_t *)malloc(qual_cap * sizeof(seq_t));
        qual_len = 0;
        next_candidate = 2;
    }

    while (qual_len < (size_t)n) {
        /* Find next prime candidate */
        while (!is_prime(next_candidate)) {
            next_candidate += (next_candidate == 2 ? 1 : 2);
        }
        idx_t p = next_candidate;

        /* Advance candidate for the next search */
        next_candidate += (p == 2 ? 1 : 2);

        /* Sum of digits of p */
        seq_t sum = digit_sum((seq_t)p);

        /* First (most significant) digit of p */
        idx_t temp = p;
        while (temp >= 10)
            temp /= 10;
        seq_t first_digit = (seq_t)temp;

        /* Difference sum - first_digit */
        seq_t diff = sum - first_digit;

        /* Check whether diff is > 1 and prime */
        if (diff > (seq_t)1 && is_prime_big(diff)) {
            /* Store qualifying prime */
            if (qual_len == qual_cap) {
                qual_cap <<= 1;
                qual_primes = (seq_t *)realloc(qual_primes,
                                               qual_cap * sizeof(seq_t));
            }
            qual_primes[qual_len++] = (seq_t)p;
        }
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    ensure_qualifiers(n);
    return qual_primes[n - 1];  /* offset 1 */
}
