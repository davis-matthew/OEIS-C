/*
 * A050723  Numbers k such that the decimal expansion of 2^k contains no pair
 *          of consecutive equal digits (probably finite).
 * https://oeis.org/A050723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search implementation of OEIS A050723:
   numbers k such that the decimal expansion of 2^k contains no
   pair of consecutive equal digits.  The function builds an
   incremental cache of qualifying exponents. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static state held across calls */
    static bool initialized = false;

    static idx_t cur_k = 0;          /* current exponent of 2^cur_k          */
    static uint8_t *digits = NULL;   /* decimal digits of 2^cur_k, LSB first */
    static size_t dlen   = 0;        /* current number of digits             */
    static size_t dcap   = 0;        /* allocated capacity for digits        */

    static idx_t *cache = NULL;      /* list of exponents satisfying the rule*/
    static size_t csize = 0;         /* number of cached exponents           */
    static size_t ccap   = 0;        /* allocated capacity for cache         */

    /* ------------------------------------------------------------------ */
    /* one‑time initialisation                                           */
    /* ------------------------------------------------------------------ */
    if (!initialized) {
        /* initial allocation for decimal digits */
        dcap = 8;
        digits = (uint8_t *)malloc(dcap * sizeof(uint8_t));
        digits[0] = 1;   /* 2^0 = 1 */
        dlen = 1;

        /* initial allocation for cache */
        ccap = 8;
        cache = (idx_t *)malloc(ccap * sizeof(idx_t));

        cur_k = 0;
        initialized = true;
    }

    /* ------------------------------------------------------------------ */
    /* extend cache until it holds at least n elements                    */
    /* ------------------------------------------------------------------ */
    while (csize < n) {
        /* test whether current 2^cur_k has no consecutive equal digits */
        bool ok = true;
        for (size_t i = 0; i + 1 < dlen; ++i) {
            if (digits[i] == digits[i + 1]) {
                ok = false;
                break;
            }
        }

        if (ok) {
            /* grow cache if needed */
            if (csize == ccap) {
                ccap *= 2;
                cache = (idx_t *)realloc(cache, ccap * sizeof(idx_t));
            }
            cache[csize++] = cur_k;
        }

        /* generate next power of two: 2^(cur_k+1) */
        uint32_t carry = 0;
        for (size_t i = 0; i < dlen; ++i) {
            uint32_t tmp = (uint32_t)digits[i] * 2 + carry;
            digits[i] = (uint8_t)(tmp % 10);
            carry = tmp / 10;
        }
        while (carry) {
            if (dlen == dcap) {
                dcap *= 2;
                digits = (uint8_t *)realloc(digits, dcap * sizeof(uint8_t));
            }
            digits[dlen++] = (uint8_t)(carry % 10);
            carry /= 10;
        }
        ++cur_k;
    }

    return (seq_t)cache[n - 1];
}
