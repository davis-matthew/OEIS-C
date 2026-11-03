/*
 * A037178  Longest cycle when squaring modulo n-th prime.
 * https://oeis.org/A037178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = order of 2 modulo k, where k = odd part of (prime(n) - 1)
 * Range:     u8 n <= 54, u16 n <= 6725, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9306 (2026-11-01)
 * Helpers:   factorize, odd_part, powmod, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation note: 
 *  For each prime p = prime(n) we compute k = odd_part(p-1).  
 *  Then we find the multiplicative order of 2 modulo k.  
 *  The order divides φ(k), so we factor φ(k) and repeatedly test  
 *  divisibility by each prime factor until no further reduction is possible. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <stdlib.h>
#include <string.h>
#include "../utils/factorize.h"
#include "../utils/odd_part.h"
#include "../utils/powmod.h"
#include "../utils/totient_seq.h"

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

/* Compute A037178: the multiplicative order of 2 modulo the odd part of (prime(n)-1). */
seq_t a(idx_t n)
{
    /* get the n-th unsigned prime */
    uint32_t p_uint32 = get_prime(n);
    seq_t p = (seq_t)p_uint32;

    /* compute odd part of p-1 */
    seq_t k_seq = odd_part(p - 1);
    uint64_t k = (uint64_t)k_seq;          /* k fits into 64 bits for the expected n-range */

    /* special case: order of 2 modulo 1 is defined as 1 */
    if (k == 1) return (seq_t)1;

    /* totient of k */
    seq_t phi_seq = totient_seq(k_seq);
    uint64_t phi = (uint64_t)phi_seq;     /* phi(k) <= k-1 <= 1e5 for the relevant range */

    /* factorize phi to obtain its prime divisors */
    uint64_t primes[64];
    int exps[64];                /* exponents are unused, but factorize wants them */
    size_t cnt = factorize(phi, primes, exps);

    /* start with the maximum possible order: phi */
    uint64_t ord = phi;

    /* Try to reduce ord by each prime factor of phi */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t f = primes[i];
        while (ord % f == 0) {
            uint64_t test = ord / f;
            /* if 2^test ≡ 1 (mod k), then we can reduce the order */
            if (powmod((seq_t)2, test, k_seq) == (seq_t)1) {
                ord = test;
            } else {
                break;
            }
        }
    }

    return (seq_t)ord;
}
