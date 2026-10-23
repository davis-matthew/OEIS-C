/*
 * A158080  Primes p = prime(n) such that the largest even digit of n equals
 *          the largest even digit of p.
 * https://oeis.org/A158080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) is the nth prime whose index’s largest even digit equals
 *            the prime’s largest even digit. We search successive indices and
 *            primes, caching results for repeated calls
 * Range:     u8 n <= 2, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   get_digits, is_prime, largest_even_digit_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Utilities */
#include "../utils/is_prime.h"
#include "../utils/get_digits.h"
#include "../utils/largest_even_digit_idx.h"

/* Helper: largest even digit of a seq_t prime */
static int largest_even_digit_seq(seq_t x)
{
    uint8_t buf[1024];          /* enough for all practical sizes */
    size_t len;
    get_digits(x, buf, &len);

    int max = -1;
    for (size_t i = 0; i < len; ++i) {
        uint8_t d = buf[i];
        if ((d & 1) == 0 && d > max)
            max = d;
    }
    return max;
}

/* Prime list maintenance */
static uint64_t *primes      = NULL;
static size_t   primes_cap   = 0;
static size_t   primes_cnt   = 0;

/* Ensure we have at least 'cnt' primes in the list. */
static void ensure_primes_upto(size_t cnt)
{
    if (primes_cnt >= cnt)
        return;

    /* grow array if needed */
    if (primes_cap == 0) {
        primes_cap = 100;
        primes     = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
        if (!primes) abort();
    }

    while (primes_cnt < cnt) {
        uint64_t cand;
        if (primes_cnt == 0) {
            cand = 2;
        } else {
            cand = primes[primes_cnt - 1] + 1;
        }

        for (;;) {
            if (is_prime(cand)) {
                if (primes_cnt == primes_cap) {
                    primes_cap *= 2;
                    uint64_t *tmp = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
                    if (!tmp) abort();
                    primes = tmp;
                }
                primes[primes_cnt++] = cand;
                break;
            }
            ++cand;
        }
    }
}

/* Sequence cache */
static seq_t  *seq_cache  = NULL;
static size_t  seq_cap    = 0;
static size_t  seq_len    = 0;

/* Index of the next number to test (1‑based). */
static uint64_t next_candidate = 1;

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for offset 1, but safe */

    /* Produce terms until we have at least n of them. */
    while (seq_len < (size_t)n) {
        /* Examine the next index candidate. */
        int idx_even = largest_even_digit_idx(next_candidate);
        if (idx_even >= 0) {
            /* Ensure we know the (next_candidate)-th prime. */
            ensure_primes_upto((size_t)next_candidate);
            uint64_t p_val = primes[next_candidate - 1];
            seq_t p = (seq_t)p_val;

            int prime_even = largest_even_digit_seq(p);
            if (prime_even >= 0 && idx_even == prime_even) {
                /* Store the prime. */
                if (seq_len == seq_cap) {
                    seq_cap = (seq_cap == 0 ? 128 : seq_cap * 2);
                    seq_t *tmp = (seq_t*)realloc(seq_cache, seq_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    seq_cache = tmp;
                }
                seq_cache[seq_len++] = p;
            }
        }
        ++next_candidate;
    }

    return seq_cache[n - 1];
}
