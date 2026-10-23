/*
 * A022835  a(n) = c(1)p(3) + ... + c(n)p(n+2), where c(i) = 1 if a(i-1) <
 *          p(i+2) and c(i) = -1 if a(i-1) >= p(i+2) (p(i) = primes).
 * https://oeis.org/A022835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A022835.
 * a(0) = 5
 * For n ≥ 1:  a(n) = a(n‑1) + c(n) * p(n+2)
 * where c(n) = 1  if a(n‑1) < p(n+2)
 *              -1 if a(n‑1) >= p(n+2)
 * and p(k) is the k‑th prime with 0‑based indexing (p(0) = 2). */

#include <stdlib.h>
#include <stdbool.h>

/* ======== prime table ================================================= */
static seq_t *prime_list      = NULL;   /* holds primes in 0‑based order   */
static size_t prime_capacity  = 0;      /* allocated capacity of prime_list */
static size_t prime_count     = 0;      /* currently stored primes         */

/* expands prime_list so it holds at least (needed+1) primes
   (i.e. indices 0 … needed)                                 */
static void
ensure_primes_upto_idx(idx_t needed)
{
    if (prime_count > (size_t)needed)
        return;

    /* make sure we have enough space in the array */
    if (prime_capacity <= (size_t)needed + 1) {
        size_t new_cap = prime_capacity * 2;
        if (new_cap == 0) new_cap = 64;
        while (new_cap <= (size_t)needed + 1) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(prime_list, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);                  /* allocation failure */
        prime_list = tmp;
        prime_capacity = new_cap;
    }

    /* helper variable to keep the next candidate across calls */
    static seq_t next_candidate = 0;
    static bool first_run = true;

    if (first_run) {
        /* initialise with the very first prime */
        prime_list[prime_count++] = (seq_t)2;
        next_candidate = 3;                  /* next candidate after 2 */
        first_run = false;
    }

    while (prime_count <= (size_t)needed) {
        /* test next_candidate for primality */
        bool is_prime = true;
        for (size_t j = 0; j < prime_count; ++j) {
            seq_t p = prime_list[j];
            if (p > next_candidate / p) break;   /* p^2 > next_candidate */
            if (next_candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            prime_list[prime_count++] = next_candidate;
        }
        next_candidate += 2;   /* skip even numbers */
    }
}

static seq_t
prime_at(idx_t idx)
{
    if (prime_count <= (size_t)idx)
        ensure_primes_upto_idx(idx);
    return prime_list[idx];
}

/* ======== sequence cache ============================================= */
static seq_t *seq_cache    = NULL;   /* holds computed a(n) values */
static size_t seq_cap      = 0;      /* allocated capacity of seq_cache */
static size_t seq_len      = 0;      /* number of values stored so far */

/* expands seq_cache so it can hold values up to 'needed' inclusive */
static void
ensure_seq_capacity(size_t needed)
{
    if (seq_cap > needed)
        return;
    size_t new_cap = seq_cap * 2;
    if (new_cap == 0) new_cap = 64;
    while (new_cap <= needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    seq_cache = tmp;
    seq_cap   = new_cap;
}

/* ======== public sequence function ==================================== */
seq_t
a(idx_t n)
{
    size_t target = (size_t)n;

    /* initialise cache with a(0) = 5 if empty */
    if (seq_len == 0) {
        ensure_seq_capacity(1);
        seq_cache[0] = (seq_t)5;
        seq_len = 1;
    }

    /* extend cache up to the requested index */
    if (seq_len <= target) {
        ensure_seq_capacity(target + 1);
        for (size_t i = seq_len; i <= target; ++i)
        {
            seq_t p   = prime_at((idx_t)(i + 2)); /* p(i+2)          */
            seq_t prev= seq_cache[i - 1];         /* a(i-1)          */
            if (prev < p)                         /* add             */
                seq_cache[i] = prev + p;
            else                                 /* subtract        */
                seq_cache[i] = prev - p;
        }
        seq_len = target + 1;
    }

    return seq_cache[target];
}
