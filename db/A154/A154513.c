/*
 * A154513  The prime(n)-th digit of the concatenated composites.
 * https://oeis.org/A154513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the digit at position prime(n) in the concatenation of *
 *            composite numbers (4,6,8,9,10,12,14,...)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-01)
 * Helpers:   is_prime, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* utility from the build:  uint32_t get_prime(idx_t k) */
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/num_digits0.h"

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

/* utility: digit_count(uint64_t n) -> number of decimal digits of n */

/* utility: bool is_prime(idx_t x) */

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)               /* offset is 1, return 0 for safety */
        return 0;

    /* Get the required position in the digit string: */
    idx_t pos = (idx_t) get_prime(n);    /* prime(n) (1‑based) */

    /* Scan through composite numbers and build the digit counter. */
    idx_t cumulative = 0;                 /* digits consumed so far */
    uint64_t num = 4;                      /* first composite */

    for (;;) {
        if (!is_prime(num)) {              /* composite */
            uint64_t d = num_digits0(num); /* digits of num */

            if (cumulative + d >= pos) {  /* target digit lies in num */
                idx_t offset = pos - cumulative;       /* 1‑based in num */

                /* Determine 10^(d-offset) */
                uint64_t pow10 = 1;
                for (idx_t i = 0; i < d - offset; ++i)
                    pow10 *= 10ULL;

                /* Extract the digit */
                seq_t digit = (seq_t)((num / pow10) % 10ULL);
                return digit;
            }

            cumulative += d;               /* advance counter */
        }

        ++num;                             /* next integer */
    }
}
