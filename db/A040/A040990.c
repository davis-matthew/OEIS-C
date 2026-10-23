/*
 * A040990  Primes p such that x^5 = 23 has a solution mod p.
 * https://oeis.org/A040990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that x^5 ≡ 23 (mod p) has a solution
 * Range:     u8 n <= 44, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The congruence has a solution for every prime p ≠ 23 with p % 5 ≠ 1,
   and for p ≡ 1 (mod 5) it requires 23^((p‑1)/5) ≡ 1 (mod p). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

/* --------------------------------------------------------------------- */
/* dynamic array helpers                                                 */
/* --------------------------------------------------------------------- */
static void grow_uint64(uint64_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap == 0) ? 1 : (*cap << 1);
    while (new_cap < need) new_cap <<= 1;
    *arr = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
    *cap = new_cap;
}

static void grow_seq_t(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap == 0) ? 1 : (*cap << 1);
    while (new_cap < need) new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* prime generation (trial division)                                     */
/* --------------------------------------------------------------------- */
static uint64_t *prime_list  = NULL;  /* all primes found so far */
static size_t    prime_cnt   = 0;
static size_t    prime_cap   = 0;

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > n / p) break;     /* avoid overflow */
        if (n % p == 0) return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/* cache of solved primes                                               */
/* --------------------------------------------------------------------- */
static seq_t *solved  = NULL;   /* primes with a solvable congruence */
static size_t solved_cnt = 0;
static size_t solved_cap = 0;

static uint64_t cur_candidate = 2;   /* next number to test for primality */

/* --------------------------------------------------------------------- */
/* ensure we have at least "need" solved primes                           */
/* --------------------------------------------------------------------- */
static void ensure_solved(size_t need)
{
    while (solved_cnt < need) {
        /* find next prime */
        while (!is_prime_uint64(cur_candidate))
            ++cur_candidate;

        /* add to prime list */
        grow_uint64(&prime_list, &prime_cap, prime_cnt + 1);
        prime_list[prime_cnt++] = cur_candidate;

        /* check solvability */
        bool solvable;
        if (cur_candidate == 23) {
            solvable = true;
        } else if (cur_candidate % 5 != 1) {
            solvable = true;
        } else {
            uint64_t exp = (cur_candidate - 1) / 5;
            seq_t res = mod_pow((seq_t)23, exp, (seq_t)cur_candidate);
            solvable = (res == (seq_t)1);
        }

        if (solvable) {
            grow_seq_t(&solved, &solved_cap, solved_cnt + 1);
            solved[solved_cnt++] = (seq_t)cur_candidate;
        }

        ++cur_candidate;   /* advance to next candidate */
    }
}

/* --------------------------------------------------------------------- */
/* public interface                                                     */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;     /* out of range */
    ensure_solved((size_t)n);
    return solved[n - 1];
}
