/*
 * A154768  Numbers n where the largest and smallest decimal digit of prime(n)
 *          are both even.
 * https://oeis.org/A154768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   good_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function: a(n) returns the n‑th positive integer k such that the largest
   and smallest decimal digits of the k‑th prime are both even.  The
   implementation lazily generates primes and stores the indices of those
   that satisfy the digit condition.  The result is returned as a seq_t. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/good_digits.h"

/* ---------- Prime generation ---------- */
static uint64_t *prime_vals = NULL;   /* array of found primes (1‑based) */
static size_t prime_cap = 0;          /* allocated capacity */
static size_t prime_cnt = 0;          /* number of primes stored */
static uint64_t next_candidate = 2;   /* next integer to test for primality */

/* Ensure prime_vals has at least need entries. */
static void ensure_prime_cap(size_t need)
{
    if (prime_cap >= need) return;
    size_t new_cap = prime_cap ? prime_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    uint64_t *tmp = (uint64_t*)realloc(prime_vals, new_cap * sizeof *prime_vals);
    prime_vals = tmp;
    prime_cap  = new_cap;
}

/* Append a new prime to prime_vals. */
static void push_prime(uint64_t p)
{
    ensure_prime_cap(prime_cnt + 1);
    prime_vals[prime_cnt++] = p;
}

/* Generate the next prime and store it in prime_vals. */
static void generate_next_prime(void)
{
    while (1)
    {
        uint64_t cand = next_candidate;

        /* Handle 2 separately. */
        if (cand == 2)
        {
            push_prime(2);
            next_candidate = 3;
            break;
        }

        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i)
        {
            uint64_t p = prime_vals[i];
            if (p * p > cand) break;
            if (cand % p == 0)
            {
                is_prime = false;
                break;
            }
        }

        if (is_prime)
        {
            push_prime(cand);
            next_candidate = cand + 2;   /* only odd candidates after 2 */
            break;
        }
        next_candidate += 2;
    }
}

/* ---------- Good index storage ---------- */
static idx_t *good_idxs = NULL;  /* indices k where prime(k) has even min/max digits */
static size_t good_cap = 0;
static size_t good_cnt = 0;

static void ensure_good_cap(size_t need)
{
    if (good_cap >= need) return;
    size_t new_cap = good_cap ? good_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    idx_t *tmp = (idx_t*)realloc(good_idxs, new_cap * sizeof *good_idxs);
    good_idxs = tmp;
    good_cap  = new_cap;
}

static void push_good_idx(idx_t idx)
{
    ensure_good_cap(good_cnt + 1);
    good_idxs[good_cnt++] = idx;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined but harmless */

    /* Generate primes and good indices until we have the n‑th one. */
    while (good_cnt < (size_t)n)
    {
        generate_next_prime();               /* new prime stored at prime_vals[prime_cnt-1] */
        idx_t prime_index = (idx_t)prime_cnt; /* 1‑based index of the prime */

        if (good_digits(prime_vals[prime_cnt - 1]))
            push_good_idx(prime_index);
    }

    return (seq_t)good_idxs[n - 1];
}
