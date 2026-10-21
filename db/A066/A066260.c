/*
 * A066260  In the prime factorization of n replace the k-th prime with the
 *          k-th composite number, k > 0.
 * https://oeis.org/A066260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, changed
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*********************************************************************
 *  OEIS A066260 – “Replace the k-th prime factor with the k-th composite”
 *
 *  a(n) = ∏  (k-th composite) ^ e
 *  where the product is over the prime factors p^e of n and k is the
 *  position of p in the ascending list of primes (2,3,5,7,...).
 *
 *  The implementation generates primes and composites on demand,
 *  without using any external helper functions, and is fully
 *  self‑contained.  It satisfies the requirements for any seq_t type
 *  and any idx_t type (unsigned 64‑bit index).
 *********************************************************************/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Forward declaration ---------------------------------------------------*/

seq_t a(idx_t n);

/* -----------------------------------------------------------------------*/
/*  Prime management -----------------------------------------------------*/

static uint32_t *primes = NULL;          /* list of primes (ascending)          */
static size_t   primes_len  = 0;         /* number of primes stored             */
static size_t   primes_cap  = 0;         /* allocated capacity                  */
static uint32_t next_prime  = 2;         /* next integer to test for primality */

/* Ensure that the prime list contains all primes ≤ n */
static void ensure_primes_up_to(uint32_t n)
{
    if (n < next_prime) return;

    for (uint32_t x = next_prime; x <= n; ++x) {
        bool is_prime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t sq = (uint64_t)primes[i] * primes[i];
            if (sq > x) break;
            if (x % primes[i] == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (primes_len >= primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 64;
                uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
                if (!tmp) exit(EXIT_FAILURE);   /* out of memory */
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_len++] = x;
        }
    }
    next_prime = n + 1;
}

/* -----------------------------------------------------------------------*/
/*  Composite management -------------------------------------------------*/

static seq_t *composites = NULL;       /* k‑th composite for k ≥ 1          */
static size_t composites_len = 0;       /* stored composites                  */
static size_t composites_cap = 0;       /* allocated capacity                  */
static uint32_t comp_candidate   = 4;   /* next integer to test for composite  */

/* Test if a number is prime using the current prime list */
static bool is_prime_uint32(uint32_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t sq = (uint64_t)primes[i] * primes[i];
        if (sq > x) break;
        if (x % primes[i] == 0) return false;
    }
    return true;
}

/* Return the k‑th composite (k ≥ 1) */
static seq_t composite_of_index(idx_t k)
{
    while (composites_len < (size_t)k) {
        /* Find next composite number */
        while (is_prime_uint32(comp_candidate)) ++comp_candidate;
        /* Store it */
        if (composites_len >= composites_cap) {
            size_t new_cap = composites_cap ? composites_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(composites, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            composites = tmp;
            composites_cap = new_cap;
        }
        composites[composites_len++] = (seq_t)comp_candidate;
        ++comp_candidate;
    }
    return composites[k - 1];
}

/* -----------------------------------------------------------------------*/
/*  Main sequence computation -------------------------------------------*/

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n < 1, but harmless */

    uint32_t nn = (uint32_t)n;
    ensure_primes_up_to(nn);        /* primes up to n are available */

    seq_t result = 1;
    uint32_t remainder = nn;

    for (size_t i = 0; i < primes_len; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > remainder) break;
        if (remainder % p == 0) {
            int exp = 0;
            while (remainder % p == 0) {
                remainder /= p;
                ++exp;
            }
            seq_t comp = composite_of_index(i + 1);   /* prime index is i+1 */
            for (int e = 0; e < exp; ++e) result *= comp;
        }
    }

    if (remainder > 1) {                    /* a remaining prime factor > sqrt(nn) */
        ensure_primes_up_to(remainder);
        /* find its index via binary search */
        size_t lo = 0, hi = primes_len;
        size_t idx = primes_len;
        while (lo < hi) {
            size_t mid = (lo + hi) / 2;
            if (primes[mid] == remainder) { idx = mid; break; }
            if (primes[mid] < remainder) lo = mid + 1;
            else hi = mid;
        }
        if (idx == primes_len) {
            /* This should not happen: prime not found */
            exit(EXIT_FAILURE);
        }
        seq_t comp = composite_of_index(idx + 1);
        result *= comp;                /* exponent 1 */
    }

    return result;
}
