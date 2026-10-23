/*
 * A144315  Prime numbers p such that p - 1 is the fourth a-figurate number,
 *          eighth b-figurate number and possibly sixteenth c-figurate number
 *          for some a, b and c and not a d-figurate number for any nontrivial
 *          d.
 * https://oeis.org/A144315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 30, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   floor_sqrt, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This code defines a(idx_t n) returning the n‑th prime p such that
 * p−1 satisfies the following:
 *   • p−1 is the 4th a‑figurate number  (i.e. P(a,4)=p−1 for some a>2)
 *   • p−1 is the 8th b‑figurate number  (i.e. P(b,8)=p−1 for some b>2)
 *   • p−1 may also be the 16th c‑figurate number.
 *   • p−1 is not any other d‑figurate number for d>2.
 *     (no further m‑figurate representation exists except for m=4,8,16)
 *
 * The routine searches primes in ascending order and memoises results.
 * It uses the utility functions next_prime() and floor_sqrt() which
 * are provided elsewhere in the test harness.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/next_prime.h"
#include "../utils/floor_sqrt.h"

/* memoised cache of terms */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

/* the next integer to start a prime search from (last tested+1) */
static uint64_t prime_start = 65;  /* p≡65 (mod 84) is necessary */

/* grow the cache array as needed */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = (idx_t)new_cap;
}

/*
 * Helper: returns true if a candidate integer (p-1) has some
 * figurate representation with index m other than 4,8,16.
 * The formula comes from solving
 *     n = ((s-2)m^2 - (s-4)m)/2
 * for integer s>2.  This is equivalent to
 *     s = (2n + 2m^2 - 4m) / (m^2 - m)
 */
static bool has_extra_representation(seq_t n)
{
    /* upper bound for m: m(m-1) <= 2n  ->  m <= sqrt(2n)+1 */
    seq_t two_n = n << 1;
    seq_t mmax_seq = floor_sqrt(two_n);
    idx_t mmax = (idx_t)mmax_seq;

    for (idx_t m = 3; m <= mmax; ++m)
    {
        if (m == 4 || m == 8 || m == 16) continue;  /* skip required indices */

        seq_t mseq   = (seq_t)m;
        seq_t denom  = mseq * mseq - mseq;                   /* m(m-1)   */
        seq_t num    = (n << 1) + (mseq * mseq << 1) - (mseq << 2); /* 2n + 2m^2 - 4m */

        if (num % denom == 0)
        {
            seq_t shape = num / denom;
            if (shape > (seq_t)2)  /* shape must be >2 (non‑trivial) */
                return true;       /* extra representation found */
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* unused, offset starts at 1 */

    /* generate terms until we have n terms */
    while (cache_len < n)
    {
        /* find next prime candidate */
        uint64_t cand = next_prime(prime_start);
        prime_start = cand + 1;  /* next search begins after this prime */

        seq_t p = (seq_t)cand;
        seq_t n_val = p - 1;

        /* required: n ≡ 64 (mod 84) → (n+8) % 6 == 0 and (n+48) % 28 == 0 */
        if (((n_val + 8) % 6) != 0) continue;
        if (((n_val + 48) % 28) != 0) continue;

        /* optional 16th representation */
        (void)((n_val + 224) % 120);  /* provides a bool flag if needed */

        /* reject if any other figurate representation exists */
        if (has_extra_representation(n_val))
            continue;

        /* prime satisfies all conditions – store it */
        if (cache_len == cache_cap) grow_cache();
        cache[cache_len++] = p;
    }

    return cache[n - 1];
}
