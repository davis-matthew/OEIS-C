/*
 * A079952  Number of primes less than prime(n)/2.
 * https://oeis.org/A079952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = π(⌊prime(n)/2⌋)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the count of primes less than prime(n)/2 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

static size_t primes_cnt = 0;
static uint32_t *primes = NULL;

/* Estimate an upper bound for the nth prime (for n ≥ 6). */
static size_t prime_upper_bound(idx_t n) {
    if (n < 6) return 15;
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0 * nn;
    return (size_t)bound + 1;   /* add one to be safe */
}

/* Build enough primes so that the list contains at least n primes. */
static void ensure_primes(idx_t n) {
    if (n <= (idx_t)primes_cnt) return;

    size_t limit = prime_upper_bound(n);
    bool *is_prime = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) exit(1);
    for (size_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    uint32_t *new_primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!new_primes) exit(1);
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) new_primes[idx++] = (uint32_t)i;

    free(is_prime);
    free(primes);
    primes = new_primes;
    primes_cnt = cnt;
}

/* Count how many stored primes are ≤ x. */
static seq_t count_primes_leq(uint32_t x) {
    size_t left = 0, right = primes_cnt;
    while (left < right) {
        size_t mid = left + (right - left) / 2;
        if (primes[mid] <= x) left = mid + 1;
        else right = mid;
    }
    return (seq_t)left;
}

/* External function as required. */
seq_t a(idx_t n) {
    if (n < (idx_t)1) return 0;          /* offset is 1, below that is unspecified */
    ensure_primes(n);
    uint32_t pn = primes[n - 1];         /* prime(n) */
    uint32_t half = pn >> 1;             /* floor(pn/2) */
    return count_primes_leq(half);
}
