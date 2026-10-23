/*
 * A180558  Primes that become a different prime under the mapping 7 <=> 8.
 * https://oeis.org/A180558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 424, u32 n <= 1582, u64 n <= 1582,
 *            u128 n <= 1582, big any n
 * Helpers:   swap_78
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A180558: primes that become a different prime under the mapping 7 <=> 8. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/swap_78.h"

/* Forward declaration of the sequence function. */
seq_t a(idx_t n);

/* ----- static helper data ------------------------------------------------ */

/* Cache of found sequence terms. */
static seq_t *results      = NULL;
static size_t res_cap      = 0;
static size_t res_len      = 0;

/* Primes up to a fixed limit used for primality tests. */
static uint32_t *primes     = NULL;
static size_t   prime_cnt   = 0;
static size_t   prime_cap   = 0;
static bool     primes_init = false;

/* Candidate number to test for primality. */
static uint64_t next_candidate = 2;

/* ----- capacity helpers --------------------------------------------------- */

static void ensure_res_cap(size_t needed)
{
    if (needed > res_cap) {
        size_t new_cap = res_cap ? res_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(results, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        results = tmp;
        res_cap = new_cap;
    }
}

static void ensure_prime_cap(size_t needed)
{
    if (needed > prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
        if (!tmp) abort();
        primes = tmp;
        prime_cap = new_cap;
    }
}

/* ----- sieve ------------------------------------------------------------- */

static void init_primes(void)
{
    if (primes_init) return;
    primes_init = true;

    /* Choose a generous upper bound; good enough for n up to about 10 000. */
    const uint32_t LIMIT = 2000000;            /* 2 million */
    unsigned char *is_composite = (unsigned char *)calloc(LIMIT + 1, 1);
    if (!is_composite) abort();

    ensure_prime_cap(LIMIT / 10); /* rough reserve */

    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!is_composite[i]) {
            if (prime_cnt == prime_cap) ensure_prime_cap(prime_cnt + 1);
            primes[prime_cnt++] = i;
            if ((uint64_t)i * (uint64_t)i <= LIMIT) {
                for (uint64_t j = (uint64_t)i * (uint64_t)i; j <= LIMIT; j += i)
                    is_composite[(size_t)j] = 1;
            }
        }
    }
    free(is_composite);
}

/* ----- primality test (trial division using precomputed primes) -------- */

static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    init_primes();
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* ----- digit swapping ---------------------------------------------------- */

/* ----- generator ---------------------------------------------------------- */

/* Generate the next qualifying prime and store it. */
static void generate_next(void)
{
    while (1) {
        /* Skip even numbers greater than 2. */
        if (next_candidate > 2 && (next_candidate & 1) == 0) {
            ++next_candidate;
            continue;
        }
        if (is_prime_uint64(next_candidate)) {
            uint64_t swapped = swap_78(next_candidate);
            if (swapped != next_candidate && is_prime_uint64(swapped)) {
                ensure_res_cap(res_len + 1);
                results[res_len++] = (seq_t)next_candidate;
                ++next_candidate;
                return;
            }
        }
        ++next_candidate;
    }
}

/* ----- sequence function ------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, avoid UB */
    while (res_len < n) generate_next();
    return results[n - 1];
}
