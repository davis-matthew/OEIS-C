/*
 * A075518  a(n) = floor(prime(n)/4).
 * https://oeis.org/A075518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(prime(n)/4)
 * Range:     u8 n <= 54, u16 n <= 7220, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* static storage for generated primes */
static uint64_t *primes = NULL;
static size_t primes_cap = 0;
static size_t primes_len = 0;
static idx_t max_checked = 1;

/* Ensure at least n primes are stored (prime(1) = 2) */
static void ensure_primes(idx_t n)
{
    if (primes == NULL) {
        primes_cap = 16;
        primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
        primes[0] = 2;
        primes_len = 1;
        max_checked = 2;
    }

    while (primes_len < n) {
        max_checked++;
        bool is_prime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            if (p * p > max_checked) break;
            if (max_checked % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (primes_len == primes_cap) {
                primes_cap <<= 1;
                primes = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_len++] = max_checked;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* just in case; offset is 1 */
    ensure_primes(n);
    seq_t prime = (seq_t)primes[n-1];
    return prime / 4;
}
