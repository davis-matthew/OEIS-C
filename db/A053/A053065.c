/*
 * A053065  Alternately append n-th prime to beginning or end of previous
 *          term.
 * https://oeis.org/A053065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, less
 * Formula:   a(n) = a(1)=2. For n>1, * a(n) = concat(a(n-1), p_n) if n is
 *            odd, * concat(p_n, a(n-1)) if n is even, * where p_n is the n‑th
 *            prime and concat(x,y)=x·10^{digits(y)}+y
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/num_digits.h"

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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Compute A053065 */
seq_t a(idx_t n)
{
    /* The first term is simply the first prime. */
    seq_t cur = 2;

    /* Build the sequence up to n. */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t p = (seq_t)get_prime(i);          /* the i‑th prime      */
        if (i & 1U) {                           /* odd index – append */
            cur = cur * pow10((idx_t)num_digits(p)) + p;
        } else {                                /* even index – prepend */
            cur = p * pow10((idx_t)num_digits(cur)) + cur;
        }
    }
    return cur;
}
