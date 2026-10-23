/*
 * A045348  Primes congruent to {0, 1, 2, 4} mod 5.
 * https://oeis.org/A045348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A045348: primes congruent to {0,1,2,4} mod 5.
   Computes the n‑th such prime (a(1)=2) by extending a simple prime
   list and filtering on the residue.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* dynamic storage for all primes found so far */
static idx_t *all_primes = NULL;
static size_t all_count = 0;
static size_t all_cap   = 0;

/* dynamic storage for the filtered primes that belong to the sequence */
static idx_t *sel_primes = NULL;
static size_t sel_count   = 0;
static size_t sel_cap     = 0;

/* highest integer examined so far */
static idx_t current_limit = 0;

/* enlarge a dynamic array if needed */
static void ensure_capacity(size_t *cap, size_t need, idx_t **arr)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        if (new_cap < need) new_cap = need;
        *arr = (idx_t *)realloc(*arr, new_cap * sizeof(idx_t));
        if (*arr == NULL) abort();               /* out of memory */
        *cap = new_cap;
    }
}

/* primality test using trial division by previously found primes */
static bool is_prime_candidate(idx_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < all_count; ++i) {
        idx_t p = all_primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* force the lists to contain all primes up to 'limit' */
static void extend_up_to(idx_t limit)
{
    if (limit <= current_limit) return;

    for (idx_t cand = current_limit + 1; cand <= limit; ++cand) {
        if (cand < 2) continue;
        if (is_prime_candidate(cand)) {
            /* append to all_primes */
            ensure_capacity(&all_cap, all_count + 1, &all_primes);
            all_primes[all_count++] = cand;

            /* include in sequence if it satisfies the congruence */
            if (cand % 5 != 3) {
                ensure_capacity(&sel_cap, sel_count + 1, &sel_primes);
                sel_primes[sel_count++] = cand;
            }
        }
    }
    current_limit = limit;
}

/* guarantee that at least n filtered primes are known */
static void ensure_n_selected(idx_t n)
{
    if (sel_count >= n) return;
    idx_t limit = (current_limit > 10) ? current_limit * 2ull : 20ull;
    for (;;) {
        extend_up_to(limit);
        if (sel_count >= n) break;
        limit <<= 1;   /* keep doubling */
    }
}

/* ------------------------------------------------------------------ */
/* required external function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0, harmless */

    ensure_n_selected(n);
    return (seq_t)sel_primes[n - 1];
}
