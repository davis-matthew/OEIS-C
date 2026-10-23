/*
 * A145993  Primes that start a run of at least 2 consecutive primes of the
 *          form 4k+3.
 * https://oeis.org/A145993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the primes p for which p ≡ 3 (mod 4), the previous prime
 *            ≡ 1 (mod 4), and the next prime ≡ 3 (mod 4). The sequence is
 *            indexed from 1 and computed by incremental prime generation with
 *            trial division
 * Range:     u8 n <= 9, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Trivial primality test using the list of previously found primes. */
static bool is_prime(const uint64_t x, const uint64_t *primes, size_t plen)
{
    if (x < 2) return false;
    if (x == 2) return true;
    if ((x & 1ULL) == 0) return false;
    for (size_t i = 0; i < plen; ++i) {
        const uint64_t p = primes[i];
        if (p > x / p) break;
        if (x % p == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Cache of the sequence values. */
    static seq_t *seq   = NULL;
    static size_t  seq_len = 0;
    static size_t  seq_cap = 0;

    /* Cache of primes used for trial division. */
    static uint64_t *primes   = NULL;
    static size_t    primes_len = 0;
    static size_t    primes_cap = 0;

    /* The next candidate to test for primality. */
    static uint64_t next_candidate = 2;

    /* The two most recent primes. */
    static uint64_t prev      = 0;   /* previous prime */
    static uint64_t prevprev  = 0;   /* prime before previous */

    /* Initialise caches on first call. */
    if (primes_cap == 0) {
        primes_cap = 32;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    }
    if (seq_cap == 0) {
        seq_cap = 32;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }

    /* Generate enough terms to satisfy the requested index. */
    while (seq_len < (size_t)n) {

        /* Find the next prime. */
        uint64_t cand = next_candidate;
        while (true) {
            if (cand < 2) cand = 2;
            if (cand == 2 || ((cand & 1ULL) && is_prime(cand, primes, primes_len)))
                break;
            cand += (cand == 2 ? 1 : 2);
        }

        /* Append prev to the sequence if it starts a run of 4k+3 primes. */
        if (prev != 0 &&
            (prev & 3ULL) == 3ULL &&
            (prevprev != 0 && (prevprev & 3ULL) == 1ULL) &&
            (cand & 3ULL) == 3ULL) {

            /* Grow the sequence array if necessary. */
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap << 1;
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                seq = tmp;
                seq_cap = new_cap;
            }
            seq[seq_len++] = (seq_t)prev;
        }

        /* Update the prime cache. */
        if (primes_len == primes_cap) {
            size_t new_cap = primes_cap << 1;
            uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
            primes = tmp;
            primes_cap = new_cap;
        }
        primes[primes_len++] = cand;

        /* Advance the tracking variables. */
        prevprev = prev;
        prev = cand;
        next_candidate = cand + (cand == 2 ? 1 : 2);
    }

    return seq[n - 1];
}
