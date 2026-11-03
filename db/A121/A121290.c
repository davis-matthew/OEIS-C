/*
 * A121290  a(n) = (2^prime(n) - 8)/24 for n>=2.
 * https://oeis.org/A121290
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = (2^prime(n) - 8)/24 for n ≥ 2 (offset 2)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 19 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/ipow.h"

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
#if defined(__SIZEOF_INT128__)
#include <inttypes.h>
#endif

/* Return the nth term (OEIS index) of A121290.  Index n is considered
   with offset 2:  a(2) is the first term above.  For any n<2 the value
   is undefined; this implementation returns 0.  The function works
   for any seq_t; if seq_t is small enough, a 128‑bit intermediate
   is used to avoid overflow, otherwise big_pow is used for
   arbitrary‑precision types. */
seq_t a(idx_t n)
{
    /* out‑of‑range index – defined behaviour is not specified */
    if (n < 2)
        return 0;

    /* Retrieve the nth prime.  For n≥2 the smallest prime is 3. */
    uint32_t p = get_prime(n);

    /* For large exponents use the arbitrary‑precision power routine. */
    if (p >= 128) {
        seq_t val = ipow((seq_t)2, (seq_t)p);
        val -= (seq_t)8;
        return val / (seq_t)24;
    }

#if defined(__SIZEOF_INT128__)
    /* Safe for primes up to 127; the intermediate value fits in 128 bits. */
    if (p <= 127) {
        unsigned __int128 val = ((unsigned __int128)1 << p);
        val -= (unsigned __int128)8;
        val /= (unsigned __int128)24;
        return (seq_t)val;
    }
#endif

    /* Fallback for the very unlikely case of an intermediate that
       does not fit in 128 bits but seq_t is larger. */
    seq_t val = ipow((seq_t)2, (seq_t)p);
    val -= (seq_t)8;
    return val / (seq_t)24;
}
