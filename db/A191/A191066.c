/*
 * A191066  Primes p that have Kronecker symbol (p|30) = -1.
 * https://oeis.org/A191066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p with Kronecker symbol (p|30) = -1.  This is obtained
 * by enumerating primes and testing the condition
 * (p|2)*(p|3)*(p|5) = -1.  For odd primes p  (p|2) = 1 if
 * p ≡ 1 or 7 (mod 8) else -1,  (p|3) = 1 if p ≡ 1 (mod 3) else -1,
 *  (p|5) = 1 if p ≡ 1 or 4 (mod 5) else -1.
 * Primes dividing 30 (2, 3, 5) are excluded.
 * The function builds primes incrementally and memoises the
 * qualifying primes.  It works for arbitrary‑precision seq_t and
 * for 64‑bit builds. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/*--- dynamic array helpers ------------------------------------------------*/

static void grow_uint64_array(uint64_t **arr, size_t *len, size_t *cap, size_t need)
{
    if (*cap >= need)
        return;
    size_t new_cap = (*cap == 0) ? 4u : (*cap << 1);
    while (new_cap < need)
        new_cap <<= 1;
    uint64_t *p = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
    if (!p)
        abort();
    *arr = p;
    *cap = new_cap;
}

/*--- primality test using previously found primes -----------------------*/

static bool is_prime_uint64(uint64_t cand, uint64_t *primes, size_t prime_len)
{
    if (cand < 2)
        return false;
    if (cand == 2 || cand == 3)
        return true;
    if ((cand & 1ULL) == 0)
        return false;
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = primes[i];
        uint64_t pp = p * p;
        if (pp > cand)
            break;
        if (cand % p == 0)
            return false;
    }
    return true;
}

/*--- storage of all found primes ----------------------------------------*/

static uint64_t *global_primes   = NULL;
static size_t     global_len     = 0;
static size_t     global_cap     = 0;

static void add_global_prime(uint64_t p)
{
    if (global_len == global_cap)
        grow_uint64_array(&global_primes, &global_len, &global_cap, global_len + 1);
    global_primes[global_len++] = p;
}

/*--- storage of primes satisfying the Kronecker condition ---------------*/

static uint64_t *good_primes   = NULL;
static size_t     good_len     = 0;
static size_t     good_cap     = 0;

static void add_good_prime(uint64_t p)
{
    if (good_len == good_cap)
        grow_uint64_array(&good_primes, &good_len, &good_cap, good_len + 1);
    good_primes[good_len++] = p;
}

/*--- check Kronecker condition ------------------------------------------*/

static inline bool is_good_prime(uint64_t p)
{
    if (p <= 5)
        return false;                                 /* 2,3,5 excluded */
    if ((p % 2ULL) == 0 || (p % 3ULL) == 0 || (p % 5ULL) == 0)
        return false;                                 /* only odd primes coprime to 30 */

    int s2 = ((p & 7ULL) == 1 || (p & 7ULL) == 7) ? 1 : -1;
    int s3 = (p % 3ULL == 1ULL) ? 1 : -1;
    int s5 = ((p % 5ULL == 1ULL) || (p % 5ULL == 4ULL)) ? 1 : -1;
    return (s2 * s3 * s5) == -1;
}

/*--- state for incremental enumeration -----------------------------------*/

static uint64_t next_candidate = 2;            /* next number to test */

/*--- ensure at least 'need' qualifying primes are present ----------------*/

static void ensure_good_primes(size_t need)
{
    while (good_len < need) {
        uint64_t cand = next_candidate++;
        if (!is_prime_uint64(cand, global_primes, global_len))
            continue;                    /* not a prime, skip */

        add_global_prime(cand);          /* cache the prime */

        if (is_good_prime(cand))
            add_good_prime(cand);
    }
}

/*--- sequence function ---------------------------------------------------*/

seq_t a(idx_t n)
{
    if (n == 0)
        abort();                          /* sequence defined for n ≥ 1 */

    ensure_good_primes((size_t)n);

    return (seq_t)good_primes[(size_t)n - 1];
}
