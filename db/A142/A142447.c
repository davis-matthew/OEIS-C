/*
 * A142447  Primes congruent to 39 mod 49.
 * https://oeis.org/A142447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 119, u32 n <= 120, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq, grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by searching for primes p such that p ≡ 39 (mod 49). */
#include <stddef.h>
#include "../utils/is_prime_big.h"        /* is_prime_big(seq_t n) */
#include "../utils/alloc_seq.h"          /* alloc_seq(size_t) */
#include "../utils/grow_seq.h"           /* grow_seq(seq_t **, size_t *, size_t) */

/* Return the nth prime congruent to 39 modulo 49 (1‑based index). */
seq_t a(idx_t n)
{
    static seq_t *primes = NULL;   /* array of found primes          */
    static size_t  cap   = 0;      /* current allocated capacity     */
    static size_t  len   = 0;      /* number of primes stored         */
    static idx_t   cur_k = 0;      /* next candidate index (k)       */

    /* ensure array is allocated */
    if (primes == NULL) {
        cap    = 8;
        primes = alloc_seq(cap);
    }

    /* generate primes until we have enough */
    while (len < n) {
        /* candidate = 49*k + 39 */
        seq_t cand = 49 * (seq_t)cur_k + 39;
        if (is_prime_big(cand)) {
            /* grow array if necessary */
            if (len == cap) grow_seq(&primes, &cap, cap * 2);
            primes[len++] = cand;
        }
        ++cur_k;
    }

    return primes[n - 1];
}
