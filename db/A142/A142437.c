/*
 * A142437  Primes congruent to 27 mod 49.
 * https://oeis.org/A142437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   count primes p such that p ≡ 27 (mod 49). The function returns
 *            the n‑th such prime for n ≥ 1
 * Range:     u8 n <= 1, u16 n <= 155, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

/* ensure capacity of a dynamic array of seq_t */
static inline void grow_seq(seq_t **p, size_t *cap, size_t need)
{
    if (need > *cap)
    {
        size_t new_cap = (*cap) ? (*cap) * 2 : 8;
        while (new_cap < need)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*p, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        *p = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1, undefined for n<1 */

    static seq_t *primes = NULL;         /* cached primes found so far   */
    static size_t   primes_cap = 0;      /* capacity of the array        */
    static idx_t    primes_cnt = 0;      /* number of primes stored      */
    static idx_t    last_k = 0;          /* last k processed (for next candidate) */

    /* keep generating candidates until we have at least n primes */
    while (primes_cnt < n)
    {
        /* candidate of the form 49*k + 27, for k = last_k, last_k+1, …   */
        seq_t candidate = (seq_t)49 * (seq_t)last_k + (seq_t)27;

        /* skip numbers ≤ 1 (although 27 never occurs) */
        if (candidate > 1 && is_prime((idx_t)candidate))
        {
            grow_seq(&primes, &primes_cap, primes_cnt + 1);
            primes[primes_cnt++] = candidate;
        }
        last_k++;   /* advance to next k */
    }
    /* primes is 0‑based, a(1) is primes[0] */
    return primes[n - 1];
}
