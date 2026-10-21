/*
 * A205712  Numbers k for which 9 divides prime(k)-prime(j) for some j<k; each
 *          k occurs once for each such j.
 * https://oeis.org/A205712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   grow_seq, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth element of OEIS sequence A205712.
   The sequence is defined as the multiset of indices k for which
   there exists a j<k such that 9 divides prime(k)-prime(j);
   each such pair contributes a copy of k.
   The implementation builds primes on demand, keeps a count of how many
   earlier primes share the same residue modulo 9, and uses a cumulative
   count array to answer queries in O(1) time after the necessary primes
   have been generated. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"   /* is_prime_small(uint64_t) -> bool */
#include "../utils/grow_seq.h"        /* void grow_seq(seq_t **, size_t *, size_t) */

/* Ensure that a uint32_t array has enough capacity for at least 'need' elements */
static void ensure_prime_cap(size_t need, uint32_t **arr, size_t *cap)
{
    if (*cap < need) {
        size_t newcap = (*cap == 0) ? 16 : *cap;
        while (newcap < need) newcap <<= 1;
        *arr = (uint32_t *)realloc(*arr, newcap * sizeof(uint32_t));
        *cap = newcap;
    }
}

/* Generate the next prime after the value pointed to by last. */
static uint32_t next_prime(uint32_t *last)
{
    uint64_t cand = (uint64_t)(*last) + 1;
    while (!is_prime_small(cand))
        ++cand;
    *last = (uint32_t)cand;
    return (uint32_t)cand;
}

seq_t a(idx_t n)
{
    /* static state shared between calls */
    static uint32_t *primes = NULL;
    static size_t prime_cap = 0, prime_cnt = 0;
    static uint32_t last_checked = 1;    /* will become first prime after increment */

    static seq_t *cum_counts = NULL;     /* cumulative counts for each prime */
    static size_t cum_cap = 0;
    static seq_t cumulative = 0;         /* total number of terms generated so far */

    static seq_t residue_cnt[9] = {0};   /* how many primes seen with each residue mod 9 */

    /* Generate primes until we have produced at least n terms */
    while (cumulative < (seq_t)n) {
        uint32_t p = next_prime(&last_checked);

        /* Ensure the primes array can hold the new prime */
        ensure_prime_cap(prime_cnt + 1, &primes, &prime_cap);
        primes[prime_cnt] = p;

        /* Count how many earlier primes share the same residue modulo 9 */
        uint32_t r = p % 9;
        seq_t earlier = residue_cnt[r];
        residue_cnt[r]++;

        /* Update cumulative count and store it for this prime index */
        cumulative += earlier;
        grow_seq(&cum_counts, &cum_cap, prime_cnt + 1);
        cum_counts[prime_cnt] = cumulative;

        prime_cnt++;
    }

    /* Binary search to find the prime index that contains the nth term */
    size_t lo = 0, hi = prime_cnt - 1, mid;
    while (lo < hi) {
        mid = (lo + hi) >> 1;
        if (cum_counts[mid] >= (seq_t)n)
            hi = mid;
        else
            lo = mid + 1;
    }

    /* Primes are 1‑based indexed, so add 1 to the array index */
    return (seq_t)(lo + 1);
}
