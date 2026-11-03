/*
 * A127354  Digit count of prime(n). The digit count numerically summarizes
 *          the frequency of digits 0 through 9 in that order when they occur
 *          in a number.
 * https://oeis.org/A127354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digit count of the n‑th prime, i.e. A047842(A000040(n))
 * Range:     u8 n <= 5, u16 n <= 26, u32 n <= 9802, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 56 terms through n = 9860 (2026-11-01)
 * Helpers:   digit_counts, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/digit_counts.h"
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

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so caller should use n >= 1. */
    if (n == 0)
        return 0;                       /* undefined, return 0 as a safe default */

    /* Fetch the n‑th prime (1‑based). */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    /* Count the decimal digits 0–9 in the prime. */
    unsigned char cnt[10] = {0};
    digit_counts(p, cnt);

    /* Build the encoded result by concatenating count‑digit pairs. */
    seq_t result = 0;
    for (int d = 0; d < 10; ++d) {
        int c = cnt[d];
        if (c == 0)
            continue;                      /* skip digits that do not appear */

        seq_t pair = ((seq_t)c) * 10 + d;   /* numeric value of count–digit pair */
        int dl   = num_digits(pair);        /* number of decimal digits in pair   */
        seq_t mul = pow10((idx_t)dl);      /* multiplier to shift the current result */

        result = result * mul + pair;
    }

    return result;
}
