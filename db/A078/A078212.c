/*
 * A078212  Smallest multiple of the n-th prime beginning and ending in n with
 *          a(1)=a(3)=0.
 * https://oeis.org/A078212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-01)
 * Helpers:   num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A078212:  for n ≥ 1, let p be the n‑th prime.
    a(n) is the smallest positive multiple of p that both begins
    and ends with the decimal representation of n.
    a(1) = a(3) = 0.  The algorithm searches for the minimal
    length k of trailing digits and uses a straightforward
    Chinese‑remainder search over the possible tail values.
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/num_digits_idx.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Return the n‑th term of A078212. */
seq_t a(idx_t n)
{
    /* The only known cases with no solution are n=1 and n=3. */
    if (n == 1 || n == 3)
        return (seq_t)0;

    /* Obtain the n‑th prime.  All primes needed up to n=10000
       are less than 1.3e5, so a 32‑bit unsigned is enough. */
    uint32_t p_uint = get_prime(n);
    seq_t p = (seq_t)p_uint;

    /* Decimal length of n. */
    idx_t d   = num_digits_idx(n);
    seq_t n_seq = (seq_t)n;

    /* Pre‑compute 10^d and its modulus modulo p. */
    seq_t pow10_d       = 1;
    seq_t pow10_d_modp  = 1 % p;
    for (idx_t i = 0; i < d; ++i) {
        pow10_d       *= (seq_t)10;
        pow10_d_modp   = (pow10_d_modp * (seq_t)10) % p;
    }

    /* Initial power of ten: 10^k with k == d (minimal possible). */
    seq_t pow10_k      = pow10_d;
    seq_t pow10_k_modp = pow10_d_modp;
    idx_t k = d;

    /* Search for the smallest k that yields a solution.  In practice
       a solution appears for k ≤ d + 5 for n ≤ 10000, but we allow
       the loop to continue until a solution is found.  For the only
       special cases 1 and 3 we returned already, so the loop will
       terminate. */
    while (true) {
        /* Start tail search with y = n and increment by 10^d. */
        seq_t y_value = n_seq;
        seq_t y_mod   = n_seq % p;

        /* We need to try all tails y = n + t*10^d with
           y_value < 10^k.  Since y_value increases by pow10_d
           each time, the loop ends automatically when the first
           candidate exceeds 10^k. */
        while (y_value < pow10_k) {
            /* Check divisibility: (n*10^k + y) % p == 0. */
            seq_t m_mod = (n_seq * pow10_k_modp + y_mod) % p;
            if (m_mod == 0) {
                /* Found the smallest multiple for this k. */
                return (n_seq * pow10_k) + y_value;
            }
            /* Advance to the next possible tail. */
            y_value += pow10_d;
            y_mod   += pow10_d_modp;
            y_mod    %= p;
        }

        /* Increase k and update the corresponding powers of ten. */
        ++k;
        pow10_k      *= (seq_t)10;
        pow10_k_modp  = (pow10_k_modp * (seq_t)10) % p;
    }
}
