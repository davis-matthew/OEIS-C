/*
 * A100658  Composite numbers with at least 2 factors that have different
 *          digital roots.
 * https://oeis.org/A100658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   digital_root_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A100658: Composite numbers with at least two prime factors that have different digital roots. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digital_root_uint32.h"

static uint32_t *primes = NULL;
static size_t prime_count = 0;
static size_t prime_cap   = 0;

/* Simple sieve of Eratosthenes that grows on demand. */
static void init_primes(void)
{
    if (prime_count != 0)
        return;

    /* Initial upper bound for primes – large enough for the required N. */
    size_t limit = 200000;                    /* 2·10⁵ prime sieve */

    char *is_comp = (char *)calloc(limit + 1, 1);          /* 0‑prime, 1‑composite */
    if (!is_comp)
        exit(1);

    prime_cap = limit / 10 + 10;               /* capacity for prime array */
    primes    = (uint32_t *)malloc(sizeof(uint32_t) * prime_cap);
    if (!primes) {
        free(is_comp);
        exit(1);
    }

    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (prime_count >= prime_cap) {
                prime_cap <<= 1;
                primes = (uint32_t *)realloc(primes, sizeof(uint32_t) * prime_cap);
                if (!primes) {
                    free(is_comp);
                    exit(1);
                }
            }
            primes[prime_count++] = (uint32_t)i;

            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    free(is_comp);
}

/* Test whether n has at least two distinct prime factors with different digital roots. */
static bool qualifies(uint64_t n)
{
    uint64_t m = n;
    uint32_t first_dr = 0;
    size_t distinct = 0;

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > m)
            break;
        if (m % p == 0) {
            ++distinct;
            uint32_t dr = digital_root_uint32(p);
            if (distinct == 1)
                first_dr = dr;
            else if (dr != first_dr)
                return true;               /* found two different DRs */
            while (m % p == 0)
                m /= p;
        }
    }

    if (m > 1) {        /* remaining prime factor */
        ++distinct;
        uint32_t dr = digital_root_uint32((uint32_t)m);
        if (distinct == 1)
            first_dr = dr;
        else if (dr != first_dr)
            return true;
    }
    return false;
}

/* --- OEIS sequence function ---------------------------------------------- */
seq_t a(idx_t n)
{
    init_primes();

    idx_t count   = 0;
    uint64_t cand = 3;          /* start testing from 4 */

    while (true) {
        ++cand;
        if (qualifies(cand)) {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
    }
}
