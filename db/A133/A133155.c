/*
 * A133155  Numbers formed by setting bits representing odd primes, where
 *          bit_no = (prime - 1)/2. Setting bit number b is the same as OR-ing
 *          with 2^b (i.e., bit numbers start at zero).
 * https://oeis.org/A133155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = setbit(a(n-1),(p-1)/2) where p is the n-th odd prime
 *            (i.e. a(1)=2, a(2)=6, a(3)=14, …)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   build_primes_upto, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/grow_seq.h"

/* static storage for the prime list */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;

/* static storage for the sequence cache */
static seq_t *seq_cache = NULL;
static size_t seq_capacity = 0;
static size_t seq_n = 0;           /* highest n computed, values stored in seq_cache[0..seq_n-1] */

/* estimate an upper bound for the (cnt)-th prime (cnt >= 1) */
static size_t prime_limit_estimate(size_t cnt)
{
    if (cnt < 6)
        return 15;          /* safe small limit */
    double n = (double)cnt;
    return (size_t)(n * (log(n) + log(log(n))) + 10.0);
}

/* generate primes up to an approximate upper bound; keep them in prime_list
   ensuring prime_count >= needed (i.e. at least that many primes) */
static void ensure_primes(size_t needed)
{
    if (prime_count > needed)
        return;

    size_t limit = prime_limit_estimate(needed + 1);  /* safety over‑estimate */
    uint64_t *tmp = NULL;
    size_t tmp_cnt = 0;
    build_primes_upto((idx_t)limit, &tmp, &tmp_cnt);

    /* free old cache if any */
    if (prime_list)
        free(prime_list);
    prime_list = tmp;
    prime_count = tmp_cnt;
}

/* extension of the sequence cache */
static void ensure_seq_capacity(size_t n)
{
    if (seq_capacity >= n)
        return;
    size_t new_cap = seq_capacity ? seq_capacity : 64;
    while (new_cap < n)
        new_cap <<= 1;
    grow_seq((seq_t **)&seq_cache, &seq_capacity, new_cap);
}

/* public function defined by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* out of sequence range, defined as 0 */

    size_t ns = (size_t)n;
    if (ns <= seq_n)          /* already computed */
        return seq_cache[ns - 1];

    /* ensure enough primes (need n+1 primes: 2,3,...,p_{n+1}) */
    ensure_primes(ns + 1);

    /* extend sequence cache if required */
    ensure_seq_capacity(ns);

    /* compute new values */
    seq_t last = (seq_n > 0) ? seq_cache[seq_n - 1] : (seq_t)0;
    for (size_t k = seq_n + 1; k <= ns; ++k) {
        uint64_t p = prime_list[k];          /* prime_list[0] == 2 */
        size_t bit = (p - 1) / 2;             /* bit position to set */
        seq_t val = last;
        if (bit < sizeof(seq_t) * 8)           /* avoid undefined left shift */
            val |= ((seq_t)1 << bit);
        seq_cache[k - 1] = val;
        last = val;
    }

    seq_n = ns;
    return seq_cache[ns - 1];
}
