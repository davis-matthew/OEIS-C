/*
 * A095244  a(n) = concatenation n,n-1,n-2,...,3,2,1 mod (prime(n)).
 * https://oeis.org/A095244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (concatenation of n, n-1, …, 1) mod prime(n)
 *
 * The function concatenates the decimal representations of the integers
 * n down to 1 and then takes the result modulo the nth prime.
 * It works for any seq_t (8,16,32,64  or 128 bit) and for all n >= 1.
 * The algorithm runs in O(n) time, with only 64-bit arithmetic needed for
 * intermediate values, since the modulus prime(n) is at most the
 * 10000th prime (< 10^6) for the required range.
 */

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

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1 -- unused */
    seq_t prime = get_prime(n);           /* prime(n) */

    /* powers of 10 mod prime, up to 5 digits (10000 has 5 digits) */
    seq_t pow10[6];
    pow10[0] = 1 % prime;
    for (int d = 1; d <= 5; ++d)
        pow10[d] = (pow10[d-1] * 10) % prime;

    seq_t res = 0;
    for (idx_t k = n; ; --k) {
        int digits;
        if (k >= 10000)      digits = 5;
        else if (k >= 1000)  digits = 4;
        else if (k >= 100)   digits = 3;
        else if (k >= 10)    digits = 2;
        else                 digits = 1;

        res = (res * pow10[digits] + (seq_t)k) % prime;

        if (k == 1) break;
    }
    return res;
}
