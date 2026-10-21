/*
 * A232862  Least positive integer m <= n^2/2 + 3 such that the set {prime(k)
 *          - k: k = 1,...,m} contains a complete system of residues modulo n,
 *          or 0 if such a number m does not exist.
 * https://oeis.org/A232862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 4033, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A232862
   a(n) is the least positive integer m ≤ n^2/2 + 3 such that
   { prime(k) – k : k = 1,…,m } is a complete system of residues mod n,
   or 0 if no such m exists.  The sequence is defined for n ≥ 1.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Prime storage – dynamic array, shared between calls. */
static uint64_t *prime_array = NULL;   /* array of primes */
static size_t   prime_cnt   = 0;        /* number of primes stored   */
static size_t   prime_cap   = 0;        /* capacity of array         */
static uint64_t last_checked = 2;      /* last number tested for primality */

/* Ensure prime_array has capacity at least cap.  */
static void grow_prime_array(size_t cap)
{
    if (prime_cap >= cap)
        return;
    size_t new_cap = prime_cap == 0 ? 16 : prime_cap * 2;
    while (new_cap < cap) new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(prime_array, new_cap * sizeof(uint64_t));
    if (!tmp) exit(1);                 /* out of memory */
    prime_array = tmp;
    prime_cap   = new_cap;
}

/* Append a new prime to the list.  */
static void add_prime(uint64_t p)
{
    grow_prime_array(prime_cnt + 1);
    prime_array[prime_cnt++] = p;
}

/* Test whether candidate is prime using the primes already stored. */
static bool is_prime_candidate(uint64_t cand)
{
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_array[i];
        if (p * p > cand) break;
        if (cand % p == 0) return false;
    }
    return true;
}

/* Ensure that prime_array contains at least k primes, generating more if needed. */
static void ensure_primes_count(size_t k)
{
    if (prime_cnt >= k)
        return;
    /* Prime array must start with 2. */
    if (prime_cnt == 0) {
        add_prime(2);
        last_checked = 2;
    }
    /* Generate subsequent primes. */
    for (uint64_t cand = (last_checked == 2) ? 3 : last_checked + 2; ; cand += 2) {
        last_checked = cand;
        if (is_prime_candidate(cand))
            add_prime(cand);
        if (prime_cnt >= k)
            break;
    }
}

/* ------------------------------------------------------------ */
/*  The required sequence function                                 */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                      /* undefined for n = 0 */

    /* upper bound for m is n^2/2 + 3, computed in seq_t to avoid overflow */
    uint64_t nn = (uint64_t)n;                       /* for the following arithmetic */
    seq_t bound = (seq_t)nn * (seq_t)nn / 2 + 3;

    /* bitmap of residues modulo n */
    bool *present = (bool *)calloc((size_t)n, 1);
    if (!present) return (seq_t)0;                   /* out of memory */

    size_t found = 0;

    /* iterate k = 1 .. bound, generating primes on demand */
    for (seq_t k = 1; k <= bound; ++k) {
        /* need the k-th prime (k is 1‑based) */
        ensure_primes_count((size_t)k);
        uint64_t pk = prime_array[(size_t)k - 1];

        /* compute (prime(k) – k) mod n */
        uint64_t p_mod = pk % nn;
        uint64_t k_mod = (uint64_t)k % nn;
        uint64_t r = p_mod + nn - k_mod;
        if (r >= nn) r -= nn;

        size_t idx = (size_t)r;
        if (!present[idx]) {
            present[idx] = true;
            if (++found == (size_t)n) {
                free(present);
                return k;                         /* m found */
            }
        }
    }

    free(present);
    return (seq_t)0;                                 /* bound exceeded with no solution */
}
