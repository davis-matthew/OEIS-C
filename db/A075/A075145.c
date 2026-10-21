/*
 * A075145  Prime basis of A061373.
 * https://oeis.org/A075145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A061373(p(n)), where p(n) is the n‑th prime. A061373(p) =
 *            1 + Σ( e_i · a(i) ) for p-1 = Π p_i^{e_i} (see OEIS
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/get_prime.h"

/* ----------  internal storage  ---------- */
static seq_t *a_cache   = NULL;   /* a_cache[1..len]  (index 0 unused) */
static size_t a_len     = 0;      /* largest n for which a(n) is known */
static size_t a_cap     = 0;      /* current capacity of a_cache      */

static uint32_t *prime_list = NULL;   /* primes we have already seen */
static size_t    prime_cnt  = 0;      /* number of primes stored       */
static size_t    prime_cap  = 0;      /* current capacity of prime_list */

/* ----------  utility functions  ---------- */

/* grow a_cache so that it can hold at least 'need' elements (indices 0..need-1) */
static void ensure_a_cache(size_t need)
{
    if (need <= a_cap) return;
    size_t newcap = a_cap ? a_cap << 1 : 1;
    while (newcap < need) newcap <<= 1;
    seq_t *tmp = (seq_t*)realloc(a_cache, newcap * sizeof(seq_t));
    a_cache = tmp;
    a_cap   = newcap;
}

/* grow prime_list so that it can hold at least 'need' primes */
static void ensure_prime_list(size_t need)
{
    if (need <= prime_cap) return;
    size_t newcap = prime_cap ? prime_cap << 1 : 1;
    while (newcap < need) newcap <<= 1;
    uint32_t *tmp = (uint32_t*)realloc(prime_list, newcap * sizeof(uint32_t));
    prime_list = tmp;
    prime_cap  = newcap;
}

/* binary search to find the 1‑based index of a prime already stored in prime_list.
   It is guaranteed that 'prime' is present in prime_list[0 .. prime_cnt-1].
*/
static idx_t prime_index_of(uint32_t prime)
{
    size_t lo = 0, hi = prime_cnt;  /* search in [lo, hi) */
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < prime) {
            lo = mid + 1;
        } else if (prime_list[mid] > prime) {
            hi = mid;
        } else {
            return (idx_t)(mid + 1); /* 1‑based index */
        }
    }
    return 0; /* should never happen */
}

/* ----------  sequence function  ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* the sequence is defined for n ≥ 1 */

    size_t target = (size_t)n;

    /* make sure the cache can hold all values up to 'target' */
    ensure_a_cache(target + 1);

    /* compute missing terms up to 'target' */
    for (size_t i = a_len + 1; i <= target; ++i) {
        uint32_t p = get_prime((idx_t)i);          /* i‑th prime */

        seq_t val;
        if (i == 1) {                              /* a(1) = 2 */
            val = (seq_t)2;
        } else {
            uint64_t tmp = (uint64_t)p - 1;        /* factor p-1 */
            seq_t sum = (seq_t)1;                 /* the leading '+1' */

            /* trial division by all known primes <= sqrt(tmp) */
            for (size_t j = 0; j < prime_cnt; ++j) {
                uint32_t q = prime_list[j];
                uint64_t q64 = (uint64_t)q;
                if (q64 * q64 > tmp) break;
                if (tmp % q == 0) {
                    uint64_t exp = 0;
                    do {
                        tmp /= q;
                        ++exp;
                    } while (tmp % q == 0);
                    seq_t exp_seq = (seq_t)exp;
                    sum += exp_seq * a_cache[prime_index_of(q)];
                }
            }

            /* any remaining factor > 1 is prime */
            if (tmp > 1) {
                uint32_t rem = (uint32_t)tmp;
                sum += a_cache[prime_index_of(rem)];
            }

            val = sum;
        }

        a_cache[i] = val;                /* store the new value */

        /* register the prime for future factorisations */
        prime_index_of(p);               /* no action needed here */
        ensure_prime_list(prime_cnt + 1);
        prime_list[prime_cnt++] = p;
    }

    a_len = target;                       /* update known length */
    return a_cache[target];
}
