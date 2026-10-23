/*
 * A137367  Subset of A037165 (p(n)*p(n+1)-p(n)-p(n+1)) for twin primes.
 * https://oeis.org/A137367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p*q - p - q where (p,q) is the n-th twin‑prime pair (p
 *            and q = p+2)
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 860, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Array grow helper.  The function is uses only C compatible operations
 * and works for C++ as well because the cast from void* to uint64_t* is
 * explicit. */
static void grow_u64(uint64_t **arr, size_t *cnt, size_t *cap, size_t new_cap)
{
    if (*cap < new_cap) {
        size_t newcapacity = *cap ? (*cap << 1) : 4;
        while (newcapacity < new_cap)
            newcapacity <<= 1;
        uint64_t *tmp = (uint64_t*)realloc(*arr, newcapacity * sizeof(uint64_t));
        if (!tmp) abort();                     /* memory exhausted */
        *arr    = tmp;
        *cap    = newcapacity;
    }
}

/* Global cached data ------------------------------------------------------- */
static uint64_t *primes      = NULL;   /* all primes found so far           */
static size_t   primes_cnt   = 0;       /* number of primes stored           */
static size_t   primes_cap   = 0;       /* allocated capacity of primes      */

static uint64_t *twin_first  = NULL;   /* first prime of each twin pair     */
static size_t   twin_cnt     = 0;       /* number of twin pairs stored       */
static size_t   twin_cap     = 0;       /* allocated capacity of twinbacks   */

static uint64_t next_candidate = 2;    /* integer to test for primality */

/* ------------------------------------------------------------------------- */
static void ensure_twin_primes(idx_t needed)
{
    if (needed <= twin_cnt) return;          /* already have enough           */

    while (twin_cnt < (size_t)needed) {
        /* Primality test of next_candidate by trial division with known
         * primes (fast enough for 10k–hundreds-of-thousands candidates). */
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if ((unsigned __int128)p * p > next_candidate)
                break;
            if (next_candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Store the new prime. */
            grow_u64(&primes, &primes_cnt, &primes_cap, primes_cnt + 1);
            primes[primes_cnt++] = next_candidate;

            /* If it forms a twin pair with the previous prime, store it. */
            if (primes_cnt >= 2) {
                uint64_t prev = primes[primes_cnt - 2];
                if (next_candidate - prev == 2) {
                    grow_u64(&twin_first, &twin_cnt, &twin_cap, twin_cnt + 1);
                    twin_first[twin_cnt++] = prev;
                }
            }
        }

        /* Advance to next odd number (after 2). */
        next_candidate = (next_candidate == 2 ? 3 : next_candidate + 2);
    }
}

/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1, undefined value */
        return (seq_t)0;

    ensure_twin_primes(n);

    seq_t p = (seq_t)twin_first[n - 1];
    seq_t q = p + (seq_t)2;
    return p * q - p - q;
}
