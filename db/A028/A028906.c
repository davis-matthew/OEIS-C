/*
 * A028906  Arrange digits of primes in descending order.
 * https://oeis.org/A028906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = arrange the decimal digits of the nth prime in descending
 *            order
 * Range:     u8 n <= 26, u16 n <= 1229, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9564 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is a(n) = A004186(A000040(n)) */

/* The function uses the provided helper get_prime(idx_t k) to obtain the nth prime,
 * then counts its decimal digits and rebuilds the integer with the digits sorted
 * from 9 down to 0. The result is returned in the generic integer type seq_t. */

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
    /* n is 1-based. get_prime expects 1-based index. */
    uint32_t prime = get_prime(n);           /* nth prime, fits in 32 bits for n <= 10000 */
    seq_t result = 0;

    /* Count digits of prime */
    int cnt[10] = {0};
    uint32_t tmp = prime;
    if (tmp == 0)          /* prime cannot be 0, but handle just in case */
        cnt[0] = 1;
    while (tmp > 0) {
        int d = tmp % 10;
        ++cnt[d];
        tmp /= 10;
    }

    /* Reconstruct number with digits sorted descending */
    for (int d = 9; d >= 0; --d) {
        int c = cnt[d];
        while (c-- > 0)
            result = result * 10 + d;
    }

    return result;
}
