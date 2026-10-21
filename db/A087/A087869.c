/*
 * A087869  a[n]
 *          =Abs[Abs[Prime[n]-a[n-1]]+Abs[Prime[n]-a[n-2]]+Abs[Prime[n]-a[n-3]]-Abs[Prime[n]-a[n-4]]-Abs[Prime[n]-a[n-5]]].
 * https://oeis.org/A087869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Abs[ Abs[Prime[n]-a(n-1)] + Abs[Prime[n]-a(n-2)] +
 *            Abs[Prime[n]-a(n-3)] - Abs[Prime[n]-a(n-4)] -
 *            Abs[Prime[n]-a(n-5)] ] Initial values: a(1)=1, a(2)=1, a(3)=1,
 *            a(4)=1, a(5)=10
 * Range:     u8 n <= 33, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   absdiff_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/absdiff_seq.h"

/* ---------- Prime generation (dynamic incremental sieve) ---------- */

static uint64_t *prime_list   = NULL;   /* array of primes (0‑based)      */
static size_t   prime_count   = 0;      /* number of primes stored       */
static size_t   prime_cap     = 0;      /* capacity of the prime_list    */
static uint64_t last_checked  = 1;      /* last candidate tested         */

/* grow the prime array to hold at least new_cap primes */
static void grow_primes(size_t new_cap)
{
    if (prime_cap >= new_cap)
        return;
    uint64_t *new_list = (uint64_t*)realloc(prime_list,
                                            new_cap * sizeof(uint64_t));
    if (!new_list)
        exit(1);
    prime_list = new_list;
    prime_cap = new_cap;
}

/* test if a value is prime using the current prime list */
static int is_prime_candidate(uint64_t cand)
{
    if (cand < 2)
        return 0;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > cand)
            break;
        if (cand % p == 0)
            return 0;
    }
    return 1;
}

/* ensure that at least n primes are in prime_list */
static void ensure_primes_upto_n(idx_t n)
{
    while (prime_count < (size_t)n) {
        uint64_t cand = last_checked + 1;
        last_checked = cand;
        if (is_prime_candidate(cand)) {
            grow_primes(prime_cap + 1);
            prime_list[prime_count++] = cand;
        }
    }
}

/* ---------- Sequence caching and computation ---------- */

static seq_t *seq_cache     = NULL;   /* cached sequence values   */
static size_t seq_cache_cap = 0;      /* capacity of seq_cache    */
static size_t seq_cache_len = 0;      /* highest n that is cached */

/* grow the cache array to at least new_cap elements */
static void grow_cache(size_t new_cap)
{
    if (seq_cache_cap >= new_cap)
        return;
    seq_t *new_cache = (seq_t*)realloc(seq_cache,
                                      new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(1);
    seq_cache = new_cache;
    seq_cache_cap = new_cap;
}

/* compute a(n) and keep all values up to n cached */
static void ensure_seq_cache(idx_t n)
{
    if (seq_cache_len >= (size_t)n)
        return;   /* already computed */

    grow_cache((size_t)n + 1);  /* +1 so we can use index n directly */

    for (idx_t i = (idx_t)(seq_cache_len + 1); i <= n; ++i) {
        seq_t val;
        switch (i) {
            case 1:
            case 2:
            case 3:
            case 4:
                val = (seq_t)1;
                break;
            case 5:
                val = (seq_t)10;
                break;
            default: {
                /* nth prime */
                seq_t p = (seq_t)prime_list[(size_t)(i - 1)]; /* 1‑based */

                seq_t a1 = seq_cache[(size_t)(i - 1)];
                seq_t a2 = seq_cache[(size_t)(i - 2)];
                seq_t a3 = seq_cache[(size_t)(i - 3)];
                seq_t a4 = seq_cache[(size_t)(i - 4)];
                seq_t a5 = seq_cache[(size_t)(i - 5)];

                seq_t d1 = absdiff_seq(p, a1);
                seq_t d2 = absdiff_seq(p, a2);
                seq_t d3 = absdiff_seq(p, a3);
                seq_t d4 = absdiff_seq(p, a4);
                seq_t d5 = absdiff_seq(p, a5);

                seq_t left  = d1 + d2 + d3;
                seq_t right = d4 + d5;
                val = absdiff_seq(left, right);
            }
        }
        seq_cache[(size_t)i] = val;
    }
    seq_cache_len = (size_t)n;
}

/* ---------- The required function ---------- */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;             /* undefined for n < 1 */

    ensure_primes_upto_n(n);
    ensure_seq_cache(n);
    return seq_cache[(size_t)n];
}
