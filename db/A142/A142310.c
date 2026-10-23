/*
 * A142310  Primes congruent to 41 mod 44.
 * https://oeis.org/A142310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime congruent to 41 modulo 44 (OEIS A142310)
 * Range:     u8 n <= 2, u16 n <= 318, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: incremental prime search with trial division.  The function
   keeps static tables of all primes found so far and of the primes with
   residue 41 (mod 44).  It handles arbitrary‑precision seq_t by casting
   the 64‑bit prime values to seq_t.  The code works for any build where
   seq_t, idx_t are the common integer types.  No external libraries are
   required beyond the standard headers. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---- dynamic array helpers -------------------------------------------- */
static void ensure_cap(uint64_t **arr, size_t *cnt, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
        if (!tmp) abort();                    /* memory failure */
        *arr  = tmp;
        *cap  = new_cap;
    }
}

/* ---- integer square root for uint64_t -------------------------------- */
static idx_t int_sqrt_u64(idx_t n)
{
    idx_t low = 0, high = (1ULL << 32);     /* sqrt(2^64-1) < 2^32 */
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        idx_t m2  = mid * mid;               /* mid ≤ 2^32, product fits in 64 bits */
        if (m2 == n) return mid;
        if (m2 < n) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

/* ---- static state ----------------------------------------------------- */
static uint64_t *all_primes   = NULL; /* all primes up to current point */
static size_t    all_cnt      = 0;
static size_t    all_cap      = 0;

static uint64_t *res_primes   = NULL; /* primes ≡ 41 (mod 44) */
static size_t    res_cnt      = 0;
static size_t    res_cap      = 0;

static bool      init_done    = false;
static idx_t     next_candidate = 3;   /* next odd number to test */

/* ---- find the next prime ---------------------------------------------- */
static uint64_t next_prime(void)
{
    if (!init_done) {                 /* first prime 2 */
        ensure_cap(&all_primes, &all_cnt, &all_cap, all_cnt + 1);
        all_primes[all_cnt++] = 2U;
        init_done = true;
    }

    while (true) {
        idx_t p = next_candidate;
        /* test primality by trial division using stored primes */
        bool is_prime = true;
        idx_t limit = int_sqrt_u64(p);
        for (size_t i = 0; i < all_cnt && all_primes[i] <= limit; ++i) {
            if (p % all_primes[i] == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            /* store in all-primes list */
            ensure_cap(&all_primes, &all_cnt, &all_cap, all_cnt + 1);
            all_primes[all_cnt++] = p;
            /* store in residue list if needed */
            if (p % 44U == 41U) {
                ensure_cap(&res_primes, &res_cnt, &res_cap, res_cnt + 1);
                res_primes[res_cnt++] = p;
            }
            /* advance to next odd candidate */
            next_candidate += 2;
            return (uint64_t)p;
        }
        /* not prime – try next odd number */
        next_candidate += 2;
    }
}

/* ---- public sequence function ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for offset 1 */
    /* ensure we have the n-th residue prime */
    while (res_cnt < (size_t)n) {
        (void)next_prime();
    }
    return (seq_t)res_primes[n - 1];
}
