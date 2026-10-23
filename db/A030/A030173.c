/*
 * A030173  Differences p(i)-p(j) between primes, sorted in numerical order.
 * https://oeis.org/A030173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) is the n-th smallest positive integer that is either even
 *            or one less than a prime; i.e., the n-th term of the sorted set
 *            of all differences between primes
 * Range:     u8 n <= 181, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *seq_cache = NULL;      /* Cached sequence values. */
static size_t seq_cap   = 0;          /* Capacity of seq_cache. */
static size_t seq_len   = 0;          /* Number of terms currently cached. */

static bool *prime_flag = NULL;      /* Sieve table: true if index is prime. */
static idx_t  prime_limit = 0;       /* Current size of prime_flag table. */
static idx_t  checked_max = 0;       /* Largest integer checked for inclusion. */

/* Expand prime_flag to cover at least [0, limit].  The sieve is recomputed
   from scratch each time it is expanded; this is inexpensive for the
   limits required (n <= 10000 => limit <= ~20000). */
static void expand_prime_flag(idx_t limit)
{
    if (limit <= prime_limit) return;

    bool *new_flag = (bool *)malloc((size_t)(limit + 1));
    if (!new_flag) exit(EXIT_FAILURE);

    for (idx_t i = 0; i <= limit; ++i) new_flag[i] = true;
    if (limit >= 0) new_flag[0] = false;
    if (limit >= 1) new_flag[1] = false;

    for (idx_t p = 2; (idx_t)p * p <= limit; ++p) {
        if (new_flag[p]) {
            for (idx_t mult = p * p; mult <= limit; mult += p)
                new_flag[mult] = false;
        }
    }

    free(prime_flag);
    prime_flag = new_flag;
    prime_limit = limit;
}

/* Check primality of an unsigned 64‑bit integer using the table. */
static bool is_prime_u64(uint64_t x)
{
    if (x <= (uint64_t)prime_limit) return prime_flag[(size_t)x];
    expand_prime_flag((idx_t) x);
    return prime_flag[(size_t)x];
}

/* Build the sequence until it contains at least `need` terms. */
static void ensure_sequence(idx_t need)
{
    if (need <= seq_len) return;

    /* Estimate the maximum integer we will have to consider.
       Because roughly half of all numbers are even and appear, we
       need at most about 2*need values.  Add a safety margin. */
    idx_t limit = 2 * need + 10;
    if (limit < checked_max + 10) limit = checked_max + 10;

    expand_prime_flag(limit + 10);   /* Ensure prime table covers needed values. */

    while (seq_len < need) {
        ++checked_max;
        bool include = false;
        if ((checked_max & 1) == 0) {
            include = true;                       /* even numbers always included. */
        } else if (is_prime_u64((uint64_t)checked_max + 2ULL)) {
            include = true;                       /* odd numbers where n+2 is prime. */
        }

        if (include) {
            if (seq_len >= seq_cap) {
                seq_cap = (seq_cap == 0) ? 64 : seq_cap << 1;
                seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
                if (!seq_cache) exit(EXIT_FAILURE);
            }
            seq_cache[seq_len++] = (seq_t)checked_max;
        }
    }
}

/* Return the n‑th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    /* The OEIS offset is 1: a(1) is the first term. */
    if (n == 0) return 0;            /* Undefined; return 0 as precaution. */
    ensure_sequence(n);
    return seq_cache[n - 1];
}
