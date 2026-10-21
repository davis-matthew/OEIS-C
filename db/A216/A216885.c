/*
 * A216885  Primes p such that x^47 = 2 has a solution mod p.
 * https://oeis.org/A216885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes p such that the congruence x^47 = 2 has a solution mod p.
   The algorithm enumerates primes in increasing order and tests each one.
   A prime p satisfies the condition iff
        p = 2   or
        47 ∤ (p-1)   or
        2^((p-1)/47) ≡ 1 (mod p).  */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"
#include "../utils/is_prime_small.h"

/* Return true if the prime p satisfies the condition. */
static inline bool satisfies(seq_t p)
{
    if (p == 2) return true;          /* 2^0 = 1 ≡ 2 (mod 2) trivially */
    seq_t diff = p - 1;

    /* If 47 does not divide p-1, the congruence has a solution
       by Fermat's little theorem (as explained above). */
    if (diff % 47 != 0) return true;

    /* Otherwise check 2^((p-1)/47) ≡ 1 (mod p). */
    seq_t exp = diff / 47;
    return mod_pow((seq_t)2, (uint64_t)exp, p) == 1;
}

/* Next prime after 'start'.  Uses is_prime_small for trial division. */
static seq_t next_prime(seq_t start)
{
    seq_t cand = start + 1;
    if (cand <= 2) return 2;
    if (cand % 2 == 0) cand++;          /* make odd */
    while (!is_prime_small(cand))
        cand += 2;
    return cand;
}

/* Static storage for the list of good primes. */
static seq_t *goodprimes = NULL;
static size_t good_cap   = 0;
static size_t good_sz    = 0;
static seq_t last_checked = 1;          /* last candidate examined */

/* Ensure the array has room for at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (good_cap >= need) return;
    size_t new_cap = good_cap ? good_cap * 2 : 4;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(goodprimes, new_cap * sizeof(seq_t));
    if (!tmp) abort();        /* out of memory */
    goodprimes = tmp;
    good_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* n is 1‑based; n==0 is undefined but we return 0. */
    if (n == 0) return 0;

    /* Generate good primes until we have at least n of them. */
    while (good_sz < (size_t)n) {
        last_checked = next_prime(last_checked);
        if (satisfies(last_checked)) {
            ensure_capacity(good_sz + 1);
            goodprimes[good_sz++] = last_checked;
        }
    }
    return goodprimes[n - 1];
}
