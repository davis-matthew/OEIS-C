/*
 * A137589  a(n) is the integer that results after deletion of all digits of
 *          n-th prime, except the initial digit and the final digit.
 * https://oeis.org/A137589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn, look
 * Formula:   a(n) is the integer obtained by concatenating the first and last
 *            decimal digits of the n‑th prime. For single‑digit primes the
 *            prime itself is returned. Calls are independent and may occur in
 *            any order
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9681 (2026-11-01)
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

seq_t a(idx_t n)
{
    /* n must be >= 1; behavior for n < 1 is unspecified */
    uint32_t p = get_prime(n);          /* nth prime (1‑based) */

    /* single‑digit primes are handled specially */
    if (p < 10)
        return (seq_t)p;

    uint32_t last  = p % 10;            /* trailing digit */
    uint32_t first = p;
    while (first >= 10)                 /* isolate most significant digit */
        first /= 10;

    return (seq_t)(first * 10 + last);   /* concatenate first and last digit */
}
