/*
 * A074972  a(n) == - prime(n) (modulo 20).
 * https://oeis.org/A074972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (20 - (prime(n) mod 20)) mod 20 for n ≥ 1
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 18, a(2) = 17, a(3) = 15, a(4) = 13, … (OEIS A074972) */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static uint64_t *prime_list = NULL;   /* stored primes */
static size_t    prime_count = 0;     /* number of primes stored */
static size_t    prime_cap   = 0;     /* allocation capacity */

/* Ensure that prime_list contains at least 'need' primes. */
static void
ensure_primes(size_t need)
{
    if (prime_count >= need)
        return;

    /* Allocate initial storage if this is the first call. */
    if (prime_count == 0) {
        prime_cap  = 128;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        prime_list[0] = 2;
        prime_count = 1;
    }

    /* Generate primes until we have 'need' of them. */
    uint64_t candidate = prime_list[prime_count - 1] + 1;
    while (prime_count < need) {
        /* Skip even numbers > 2. */
        if ((candidate & 1ULL) == 0 && candidate != 2)
            ++candidate;
        else {
            int is_prime = 1;
            for (size_t i = 0; i < prime_count; ++i) {
                uint64_t p = prime_list[i];
                if (p > candidate / p) /* p * p > candidate */
                    break;
                if (candidate % p == 0) {
                    is_prime = 0;
                    break;
                }
            }
            if (is_prime) {
                if (prime_count == prime_cap) {
                    prime_cap <<= 1;
                    prime_list = (uint64_t *)realloc(
                        prime_list, prime_cap * sizeof(uint64_t));
                }
                prime_list[prime_count++] = candidate;
            }
            ++candidate;
        }
    }
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* the sequence is defined for n ≥ 1 */

    /* Make sure we have the n‑th prime. */
    ensure_primes((size_t)n);

    uint64_t p = prime_list[(size_t)(n - 1)];
    uint64_t r = (20ULL - (p % 20ULL)) % 20ULL;   /* negative residue modulo 20 */

    return (seq_t)r;
}
