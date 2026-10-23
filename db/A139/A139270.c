/*
 * A139270  Twice nonprime numbers.
 * https://oeis.org/A139270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2 * A018252(n) (twice the nth nonprime)
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Global state for a dynamic sieve and prime counts */
static uint8_t  *is_prime = NULL;  /* is_prime[i] = 1 if i is prime, else 0 */
static uint32_t *pi_cnt   = NULL;  /* pi_cnt[i] = number of primes ≤ i */
static size_t    max_sieve = 0;    /* highest index currently sieved */

/* Ensure the sieve reaches at least up_to (inclusive) */
static void ensure_primes_up_to(size_t up_to)
{
    if (up_to <= max_sieve)
        return;

    /* Resize prime and pi count arrays */
    uint8_t  *new_is_prime = (uint8_t *)realloc(is_prime, (up_to + 1) * sizeof(uint8_t));
    if (!new_is_prime) exit(1);  /* fatal in this context */
    uint32_t *new_pi_cnt   = (uint32_t *)realloc(pi_cnt,   (up_to + 1) * sizeof(uint32_t));
    if (!new_pi_cnt)   exit(1);

    /* Initialise every index as prime candidate */
    for (size_t i = 0; i <= up_to; ++i)
        new_is_prime[i] = 1;

    /* 0 and 1 are not prime */
    if (up_to >= 0) new_is_prime[0] = 0;
    if (up_to >= 1) new_is_prime[1] = 0;

    /* Classic sieve of Eratosthenes */
    for (size_t p = 2; p * p <= up_to; ++p) {
        if (new_is_prime[p]) {
            for (size_t j = p * p; j <= up_to; j += p)
                new_is_prime[j] = 0;
        }
    }

    /* Build prefix prime counts */
    new_pi_cnt[0] = 0;
    for (size_t i = 1; i <= up_to; ++i)
        new_pi_cnt[i] = new_pi_cnt[i - 1] + ((i >= 2 && new_is_prime[i]) ? 1 : 0);

    /* Commit the new arrays */
    is_prime = new_is_prime;
    pi_cnt   = new_pi_cnt;
    max_sieve = up_to;
}

/* Return the nth nonprime integer (n ≥ 1) */
static size_t nth_nonprime(size_t n)
{
    /* Guarantee the sieve covers enough numbers to contain n nonprimes */
    size_t low  = 1;
    size_t high = max_sieve ? max_sieve : 1;
    ensure_primes_up_to(high);
    while (high - (size_t)pi_cnt[high] < n) {
        high = high ? high << 1 : 1;
        ensure_primes_up_to(high);
    }

    /* Binary search for the smallest m with (m - pi(m)) ≥ n */
    while (low < high) {
        size_t mid = low + ((high - low) >> 1);
        size_t count = mid - (size_t)pi_cnt[mid];
        if (count >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    size_t m = nth_nonprime((size_t)n);     /* nth nonprime integer */
    return (seq_t)m * (seq_t)2;            /* twice that number */
}
