/*
 * A138889  Primes that are not Fermat primes.
 * https://oeis.org/A138889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = nth prime that is not a Fermat prime (non‑Fermat primes)
 * Range:     u8 n <= 51, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *all_primes = NULL;    /* list of all primes found so far      */
static size_t all_cnt   = 0;         /* number of primes stored              */
static size_t all_cap   = 0;         /* current capacity of all_primes        */

static seq_t *nf_primes = NULL;     /* list of non‑Fermat primes            */
static size_t nf_cnt    = 0;         /* number of non‑Fermat primes stored   */
static size_t nf_cap    = 0;         /* current capacity of nf_primes         */

static uint64_t next_candidate = 3; /* next odd integer to test as prime     */

/* Increase the capacity of an array to hold at least 'need' elements. */
static void grow_array(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need)
        return;
    size_t new_cap = (*cap) ? (*cap) * 2 : 64;
    while (new_cap < need)
        new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* Test whether p is a Fermat prime (p == 2 or p = 2^(2^k)+1). */
static bool is_fermat_prime(uint64_t p)
{
    if (p <= 2)
        return false; /* 2 is not a Fermat prime in this context */
    uint64_t x = p - 1;
    /* p-1 must be a power of two */
    if ((x & (x - 1)) != 0)
        return false;
    unsigned exp = 0;
    while (x > 1) { /* find log2(p-1) */
        x >>= 1;
        exp++;
    }
    /* exp itself must be a power of two */
    return (exp & (exp - 1)) == 0;
}

/* Trial‑division primality test using the primes already found. */
static bool is_prime(uint64_t cand)
{
    for (size_t i = 0; i < all_cnt; i++) {
        uint64_t p = (uint64_t)all_primes[i];
        if (p * p > cand)
            break;
        if (cand % p == 0)
            return false;
    }
    return true;
}

/* Ensure that nf_primes contains at least 'n' elements (1‑based). */
static void ensure_nf_primes(idx_t n)
{
    while (nf_cnt < (size_t)n) {
        uint64_t cand = next_candidate;
        if (is_prime(cand)) {
            /* add cand to all_primes */
            grow_array(&all_primes, &all_cap, all_cnt + 1);
            all_primes[all_cnt++] = (seq_t)cand;
            /* add to non‑Fermat list if appropriate */
            if (!is_fermat_prime(cand)) {
                grow_array(&nf_primes, &nf_cap, nf_cnt + 1);
                nf_primes[nf_cnt++] = (seq_t)cand;
            }
        }
        next_candidate += 2; /* skip even numbers after 2 */
    }
}

/* External linkage: the required function. */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset starts at 1; undefined below */
        return 0;
    if (nf_cnt == 0) {          /* one‑time initialization */
        grow_array(&all_primes, &all_cap, 64);
        grow_array(&nf_primes,  &nf_cap, 64);
        all_primes[0] = (seq_t)2;         /* prime 2 */
        all_cnt = 1;
        nf_primes[0] = (seq_t)2;          /* 2 is not a Fermat prime by definition here */
        nf_cnt = 1;
        next_candidate = 3;
    }
    ensure_nf_primes(n);
    return nf_primes[n - 1];
}
