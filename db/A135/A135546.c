/*
 * A135546  Let p be the n-th prime and let g be the order of 2 mod p (see
 *          A014664). Then if g is even, a(n) = p*(2^(g/2) - 1), otherwise
 *          a(n) = 2^g - 1.
 * https://oeis.org/A135546
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined as follows. Let p be the n‑th prime (indexed
 *            from 1, the offset of the sequence is 2). Let g be the
 *            multiplicative order of 2 modulo p (i.e. the smallest positive
 *            integer such that 2^g ≡ 1 (mod p)). If g is even, a(n) = p •
 *            (2^(g/2) – 1); otherwise a(n) = 2^g – 1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 16, u64 n <= 31, u128 n <= 55,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 985 (2026-11-01)
 * Helpers:   pow2, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/pow2.h"
#include "../utils/powmod.h"

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

seq_t a(idx_t n)
{
    if (n < 2) return 0;              /* out of defined range */

    /* prime p = n‑th prime */
    uint32_t p32 = get_prime(n);
    seq_t   p   = (seq_t)p32;

    /* initial order candidate = p-1 (groups of prime modulus) */
    idx_t  ord   = (idx_t)p32 - 1;
    idx_t  order = ord;           /* will be reduced to the true order */
    idx_t  rem   = ord;           /* used for prime factorisation */

    /* factorise rem and reduce order for each prime factor */
    for (idx_t i = 2; i * i <= rem; ++i) {
        if (rem % i == 0) {
            /* reduce order by this factor as much as possible */
            while (order % i == 0 &&
                   powmod((seq_t)2, order / i, p) == (seq_t)1)
                order /= i;

            /* remove factor from rem completely */
            while (rem % i == 0) rem /= i;
        }
    }
    if (rem > 1) {                 /* remaining prime factor */
        while (order % rem == 0 &&
               powmod((seq_t)2, order / rem, p) == (seq_t)1)
            order /= rem;
    }

    /* compute the term according to the parity of the order */
    if ((order & 1ULL) == 0) {     /* even order */
        seq_t half = pow2(order / 2);          /* 2^(g/2) */
        return p * (half - (seq_t)1);
    } else {                       /* odd order */
        seq_t full = pow2(order);               /* 2^g */
        return full - (seq_t)1;
    }
}
