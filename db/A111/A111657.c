/*
 * A111657  n-th composite number appears n-th prime times.
 * https://oeis.org/A111657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = k-th composite number, where k is the least integer such
 *            that Σ_{i=1}^{k} p_i ≥ n and {p_i} are the primes in increasing
 *            order
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   grow_cap, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/grow_cap.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;                /* valid for n ≥ 1 only */

    /* ---------- static state ---------- */
    static uint64_t *primes     = NULL;  /* array of primes */
    static size_t   primes_cap  = 0, primes_cnt = 0;
    static seq_t   *cum         = NULL;  /* cumulative sum of primes */
    static size_t   cum_cap     = 0, cum_cnt = 0;
    static seq_t   *comps       = NULL;  /* array of composites */
    static size_t   comp_cap    = 0, comp_cnt = 0;

    static uint64_t next_prime     = 2ULL;  /* next candidate for prime */
    static uint64_t next_comp      = 4ULL;  /* next candidate for composite */

    /* ---------- ensure enough primes / sums for this n ---------- */
    while (cum_cnt == 0 || (seq_t)n > cum[cum_cnt - 1]) {
        /* find next prime */
        uint64_t p = next_prime;
        while (!is_prime(p)) p++;

        /* store the prime */
        grow_cap(&primes_cap, &primes_cnt, (void **)&primes, sizeof(uint64_t));
        primes[primes_cnt++] = p;

        /* update cumulative sum */
        seq_t prev_sum = (cum_cnt == 0) ? (seq_t)0 : cum[cum_cnt - 1];
        grow_cap(&cum_cap, &cum_cnt, (void **)&cum, sizeof(seq_t));
        cum[cum_cnt++] = prev_sum + (seq_t)p;

        /* advance to the following number for the next search */
        next_prime = p + 1;
    }

    /* k is the number of primes produced so far (1‑based index) */
    idx_t k = cum_cnt;

    /* ---------- ensure enough composites ---------- */
    while (comp_cnt < (size_t)k) {
        uint64_t c = next_comp;
        while (is_prime(c)) c++;          /* skip primes */

        /* store the composite */
        grow_cap(&comp_cap, &comp_cnt, (void **)&comps, sizeof(seq_t));
        comps[comp_cnt++] = (seq_t)c;

        /* next candidate starts just after this composite */
        next_comp = c + 1;
    }

    /* ---------- return the k‑th composite ---------- */
    return comps[k - 1];
}
