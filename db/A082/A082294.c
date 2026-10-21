/*
 * A082294  Numbers having exactly two square divisors > 1.
 * https://oeis.org/A082294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 3072, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A082294: Numbers having exactly two square divisors > 1.
   The code scans the natural numbers in order and keeps those that
   can be written as p^e * s with e ∈ {4,5}, s square–free and coprime
   to p.  The discovered values are memoised so that subsequent calls
   to a() are fast, no matter in which order the indices are requested. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* ---------- prime helper ------------------------------------ */
static uint64_t *prime_list     = NULL;
static size_t   prime_cnt       = 0;
static size_t   prime_cap       = 0;
static uint64_t next_prime_min  = 2;

static void extend_primes_upto(uint64_t limit)
{
    /* Grow prime_list until it contains all primes ≤ limit. */
    while (next_prime_min <= limit) {
        bool is_prime = true;
        uint64_t p = next_prime_min;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t pr = prime_list[i];
            if (pr > p / pr)   /* avoid overflow: pr*pr > p */
                break;
            if (p % pr == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 8;
                prime_list = (uint64_t *)realloc(prime_list,
                                                  prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_cnt++] = p;
        }
        ++next_prime_min;
    }
}

/* ---------- check a 64‑bit integer --------------------------- */
static bool has_two_square_divisors(uint64_t n)
{
    if (n < 16)          /* the smallest term is 16 */
        return false;

    /* Ensure we have primes up to sqrt(n). */
    uint64_t lim = 1;
    while (lim * lim < n)
        ++lim;
    extend_primes_upto(lim);

    int prime_with_exp = 0;   /* number of primes having exponent > 1 */
    int exp_of_prime   = 0;
    uint64_t temp = n;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > temp / p)
            break;
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if (e > 1) {
                ++prime_with_exp;
                exp_of_prime = e;
                if (prime_with_exp > 1)
                    return false;               /* more than one such prime */
            }
        }
    }
    if (prime_with_exp != 1)
        return false;                           /* no prime with e>1 */
    return (exp_of_prime == 4 || exp_of_prime == 5);
}

/* ---------- sequence cache ---------------------------------- */
static seq_t *seq_cache = NULL;   /* array of discovered terms */
static size_t seq_len   = 0;      /* number of terms stored */
static size_t seq_cap   = 0;

/* ---------- main accessor ----------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                                 /* not defined, safe value */

    /* Allocate cache once. */
    if (seq_cache == NULL) {
        seq_cap   = 16;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }

    /* Search forward until we have at least n terms. */
    static uint64_t next_candidate = 1;
    while (seq_len < (size_t)n) {
        if (has_two_square_divisors(next_candidate)) {
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq_cache = (seq_t *)realloc(seq_cache,
                                             seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return seq_cache[(size_t)n - 1];
}
