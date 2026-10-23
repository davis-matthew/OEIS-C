/*
 * A024705  Positions of odd numbers in A024702.
 * https://oeis.org/A024705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = index (in the list of primes) of the nth prime > 3 * that
 *            is congruent to 3 or 5 modulo 8
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static state for incremental prime generation */
static uint64_t *primes      = NULL;   /* list of found primes          */
static size_t   primes_cnt   = 0;      /* number of stored primes       */
static size_t   primes_cap   = 0;      /* allocated capacity for primes */

static idx_t *indices      = NULL;    /* indices of qualifying primes  */
static size_t indices_cnt  = 0;        /* number of stored indices      */
static size_t indices_cap  = 0;        /* allocated capacity for indices */

static uint64_t candidate   = 2;       /* next integer to test for prime */

/* Helper:  test primality of n using previously found primes up to sqrt(n) */
static bool is_prime(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Helper:  ensure enough capacity in primes array */
static void grow_primes(size_t need)
{
    if (need <= primes_cap) return;
    size_t newcap = primes_cap ? primes_cap * 2 : 64;
    while (newcap < need) newcap *= 2;
    uint64_t *newptr = (uint64_t *)realloc(primes, newcap * sizeof(uint64_t));
    if (!newptr) abort();                /* memory exhausted */
    primes = newptr;
    primes_cap = newcap;
}

/* Helper:  ensure enough capacity in indices array */
static void grow_indices(size_t need)
{
    if (need <= indices_cap) return;
    size_t newcap = indices_cap ? indices_cap * 2 : 16;
    while (newcap < need) newcap *= 2;
    idx_t *newptr = (idx_t *)realloc(indices, newcap * sizeof(idx_t));
    if (!newptr) abort();                /* memory exhausted */
    indices = newptr;
    indices_cap = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n=0, return 0 */

    /* Generate primes and qualifying indices until we have n of them */
    while (indices_cnt < (size_t)n) {
        /* Test current candidate for primality */
        if (is_prime(candidate)) {
            /* store the new prime */
            grow_primes(primes_cnt + 1);
            primes[primes_cnt] = candidate;
            ++primes_cnt;              /* primes_cnt is the 1‑based index */

            /* if prime > 3 and ≡ 3 or 5 (mod 8), record its index */
            if (candidate > 3) {
                uint64_t r = candidate % 8;
                if (r == 3 || r == 5) {
                    grow_indices(indices_cnt + 1);
                    indices[indices_cnt] = (idx_t)primes_cnt;
                    ++indices_cnt;
                }
            }
        }

        /* Advance to next candidate (skip even numbers after 2) */
        if (candidate == 2) candidate = 3;
        else candidate += 2;
    }

    return (seq_t)indices[n - 1];         /* convert idx_t to seq_t for return */
}
