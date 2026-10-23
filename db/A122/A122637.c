/*
 * A122637  a(n) = prime(n)_n.
 * https://oeis.org/A122637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = prime(n)_n
 * Range:     u8 n <= 25, u16 n <= 109, u32 n <= 894, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static data for incremental prime generation */
static uint64_t *prime_list = NULL;      /* dynamic array of primes */
static size_t prime_cap = 0;             /* current allocation size */
static size_t prime_count = 0;           /* number of primes stored */
static uint64_t candidate_prime = 2;     /* next candidate to test */

/* Ensure that at least n primes are generated and stored in prime_list. */
static void ensure_primes(idx_t n)
{
    while (prime_count < (size_t)n) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > candidate_prime) break;
            if (candidate_prime % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (prime_count == prime_cap) {
                size_t new_cap = prime_cap ? prime_cap * 2 : 256;
                uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
                if (!tmp) exit(1);                /* allocation failed */
                prime_list = tmp;
                prime_cap = new_cap;
            }
            prime_list[prime_count++] = candidate_prime;
        }
        ++candidate_prime;
    }
}

/* Main sequence function.  It computes a(n) = the nth prime,
   interpreted as a number in base n. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety for offset 1, undefined case */

    /* Make sure the nth prime is available */
    ensure_primes(n);
    uint64_t prime = prime_list[n - 1];

    /* Extract decimal digits of the prime (least‑significant first) */
    uint64_t tmp = prime;
    int digits[32];            /* enough for primes up to 10^32 */
    int len = 0;
    do {
        digits[len++] = (int)(tmp % 10);
        tmp /= 10;
    } while (tmp > 0);

    /* Interpret those digits as a base‑n number */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        result = result * (seq_t)n + (seq_t)digits[i];
    }
    return result;
}
