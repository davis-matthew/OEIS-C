/*
 * A053320  Distance between pairs of primes differing by 4.
 * https://oeis.org/A053320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p_{n+1} - p_n, where {p_i} is the increasing sequence of
 *            primes p such that p+4 is also prime (prime pairs differing by
 *            4)
 * Range:     u8 n <= 110, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint32_t *lower_primes = NULL;   /* array of lower primes of 4‑gap pairs */
static size_t lower_count = 0;          /* number of stored lower primes */
static size_t lower_cap   = 0;          /* allocated capacity */

/* Build the list of lower primes up to a fixed sieve limit (4 000 000). */
static void init_sequence(void)
{
    if (lower_primes != NULL) return;  /* already initialized */

    /* ---------- sieve to find primes up to LIMIT + 4 ---------- */
    const uint32_t LIMIT = 4000000U;           /* sufficient for n ≤ 10 000 */
    const uint32_t ARR_SIZE = LIMIT + 5U;       /* +4 to check p+4 */
    char *is_prime = (char *)malloc(ARR_SIZE);
    if (!is_prime) return;                    /* allocation failed */

    memset(is_prime, 1, ARR_SIZE);
    is_prime[0] = is_prime[1] = 0;

    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT + 4; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = (uint64_t)i * i; j <= LIMIT + 4; j += i)
                is_prime[(size_t)j] = 0;
        }
    }

    /* ---------- build list of lower primes of 4‑gap pairs ---------- */
    lower_cap = 25000U;                        /* reasonable initial cap */
    lower_primes = (uint32_t *)malloc(lower_cap * sizeof(uint32_t));
    if (!lower_primes) { free(is_prime); return; }

    for (uint32_t p = 3; p + 4U <= LIMIT; ++p) {
        if (is_prime[p] && is_prime[p + 4U]) {
            if (lower_count == lower_cap) {
                lower_cap <<= 1;
                uint32_t *tmp = (uint32_t *)realloc(lower_primes, lower_cap * sizeof(uint32_t));
                if (!tmp) break;                /* reallocation failed */
                lower_primes = tmp;
            }
            lower_primes[lower_count++] = p;
        }
    }

    /* free temporary sieve array */
    free(is_prime);
}

/* ---------------------------------------------------------------------- */
/*  seq_t a(idx_t n)  – the required function                                 */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_sequence();
        initialized = true;
    }

    /* n is 1‑based. Return the difference between successive lower primes. */
    if (n == 0 || (size_t)n >= lower_count) return 0;  /* out of bounds (should not happen) */

    size_t idx = (size_t)n;          /* index of upper member in the pair p_{n+1} */
    uint32_t diff = lower_primes[idx] - lower_primes[idx - 1];
    return (seq_t)diff;
}
