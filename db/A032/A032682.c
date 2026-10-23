/*
 * A032682  Numbers k such that k surrounded by digit '1' is a prime.
 * https://oeis.org/A032682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth non‑negative integer k such that 1·k·1
 *            (concatenation of decimal digits “1k1”) is prime
 * Range:     u8 n <= 5, u16 n <= 951, u32 n <= 60, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits, grow_seq, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function a(n) below computes this sequence on demand, caching
    previously found terms so that arbitrary order of calls is supported.
    It relies on the helper functions is_prime_big, pow10, digits, and
    grow_seq.  */

/* External declarations for helper functions. */
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/digits.h"
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

/* Cache state ------------------------------------------------------- */
static seq_t *cache = NULL;   /* array of found k values (a(n))      */
static size_t cache_cap = 0;  /* current allocation size of cache    */
static size_t cache_len = 0;  /* number of terms stored in cache     */
static idx_t k_next = 0;      /* next candidate k to test            */

/* Return the nth term of the sequence (1‑based indexing).  */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; for n ≤ 0 return 0 as a safe
       default (the build will never ask for such indices). */
    if (n == 0)
        return 0;

    /* Expand the cache until it contains at least n terms. */
    while (cache_len < n) {
        /* Candidate value k to be tested. */
        idx_t k = k_next++;

        /* Compute the number 1k1 as N = 10^(digits(k)+1) + k*10 + 1. */
        idx_t d = digits(k);                      /* number of digits of k */
        seq_t N = pow10(d + 1);                    /* 10^(d+1)            */
        N = N + (seq_t)k * 10 + 1;                 /* append k and final 1 */

        /* Test whether N is prime. */
        if (is_prime_big(N)) {
            /* Store k in the cache as the next sequence term. */
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = (seq_t)k;
        }
    }

    return cache[n - 1];
}
