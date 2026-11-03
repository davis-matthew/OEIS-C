/*
 * A126147  a(n) = floor((Product_{k=1..n-1} prime(k))/prime(n)).
 * https://oeis.org/A126147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( (Product_{k=1}^{n-1} prime(k)) / prime(n) )
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 11, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 184 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

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

#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    uint64_t p_n = get_prime(n);
    uint64_t r   = 1;   /* current remainder: product mod p_n */

#ifdef __cplusplus
    /* If seq_t is an arithmetic type, use 128‑bit arithmetic for the
       intermediate quotient to avoid overflow. Otherwise use seq_t */
    if constexpr (std::is_integral<seq_t>::value) {
        __int128 q = 0;               /* intermediate quotient */
        for (idx_t i = 1; i < n; ++i) {
            uint64_t pk   = get_prime(i);
            uint64_t prod_r = r * pk;  /* r < p_n, pk < p_n => fits 64‑bit */
            uint64_t add     = prod_r / p_n;
            r                 = prod_r % p_n;
            q                 = q * pk + add;
        }
        return (seq_t)q;
    } else {
        seq_t q = 0;
        for (idx_t i = 1; i < n; ++i) {
            uint64_t pk   = get_prime(i);
            uint64_t prod_r = r * pk;
            uint64_t add     = prod_r / p_n;
            r                 = prod_r % p_n;
            q = q * (seq_t)pk + (seq_t)add;
        }
        return q;
    }
#else
    /* C builds (seq_t is a built‑in unsigned type) */
#if defined(__SIZEOF_INT128__)
    __int128 q = 0;
    for (idx_t i = 1; i < n; ++i) {
        uint64_t pk   = get_prime(i);
        uint64_t prod_r = r * pk;
        uint64_t add     = prod_r / p_n;
        r                 = prod_r % p_n;
        q = q * pk + add;
    }
    return (seq_t)q;
#else
    /* Fallback when __int128 is not available – may overflow for large n,
       but only needed for values that still fit in seq_t. */
    seq_t prod = 1;
    for (idx_t i = 1; i < n; ++i)
        prod *= (seq_t)get_prime(i);
    return prod / (seq_t)p_n;
#endif
#endif
}
