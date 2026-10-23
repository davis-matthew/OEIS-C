/*
 * A044042  Primes congruent to {1, 2, 3, 4, 5} (mod 7).
 * https://oeis.org/A044042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A044042
 *  Primes congruent to {1, 2, 3, 4, 5} (mod 7)
 *  (i.e. all primes except 7 and primes ≡ 6 (mod 7))
 *  Offset: 1  (a(1) = 2)
 *
 *  This implementation builds the sequence on demand,
 *  storing all primes found so far in a dynamic list.
 *  It uses trial division by the previously found primes.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *prime_list   = NULL;   /* list of all primes found   */
static size_t prime_len    = 0, prime_cap = 0;
static seq_t *allowed_list = NULL;   /* list of primes satisfying the mod‑7 condition */
static size_t allowed_len  = 0, allowed_cap = 0;
static uint64_t next_candidate = 2;   /* next number to test for primality */

/* ---------- utility: dynamic array growth ---------------------------------- */
static void grow_array(seq_t **arr, size_t *len, size_t *cap, size_t new_len)
{
    if (new_len > *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        if (new_cap < new_len) new_cap = new_len;
        *arr = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
    *len = new_len;
}

static void add_prime(uint64_t p)
{
    grow_array(&prime_list, &prime_len, &prime_cap, prime_len + 1);
    prime_list[prime_len - 1] = (seq_t)p;
}

static void add_allowed(uint64_t p)
{
    grow_array(&allowed_list, &allowed_len, &allowed_cap, allowed_len + 1);
    allowed_list[allowed_len - 1] = (seq_t)p;
}

/* ---------- primality test using previously found primes ----------------- */
static bool is_prime_candidate(uint64_t n)
{
    if (n <= 1)          return false;
    if (n == 2)          return true;
    if ((n & 1ULL) == 0) return false;

    for (size_t i = 0; i < prime_len; ++i) {
        seq_t p = prime_list[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* ---------- sequence function -------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for index 0 but harmless */

    /* Generate primes and filter until we have at least n allowed primes */
    while (allowed_len < (size_t)n) {
        uint64_t cand = next_candidate;

        if (cand == 2) {
            /* 2 is prime and satisfies the mod‑7 condition */
            add_prime(2);
            add_allowed(2);
            next_candidate = 3;
            continue;
        }

        if (cand % 2 == 0) {
            /* skip even numbers (except 2 was already handled) */
            next_candidate = cand + 1;
            continue;
        }

        if (is_prime_candidate(cand)) {
            add_prime(cand);
            /* exclude 7 and numbers ≡6 (mod 7) */
            if (cand != 7 && cand % 7 != 6) {
                add_allowed(cand);
            }
        }

        next_candidate = cand + 2;   /* next odd candidate */
    }

    return allowed_list[n - 1];
}
