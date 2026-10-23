/*
 * A046448  Squarefree nonprimes with property that the concatenation of the
 *          prime factors is a palindrome.
 * https://oeis.org/A046448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   digit_count, factor_u64, is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A046448 – Squarefree non‑prime numbers whose prime factors,
    written in ascending order, concatenate to a decimal palindrome.
    a(1) = 1.
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/digit_count.h"
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    /* a(1) is defined to be 1. */
    if (n == 1) return (seq_t)1;

    idx_t found = 1;          /* 1 is already the first term */
    uint64_t cand = 2;         /* start checking from 2 upwards */

    while (1) {
        /* Factor cand into primes and exponents. */
        size_t cnt = 0;
        uint64_t primes[32];
        uint64_t exps[32];
        factor_u64(cand, primes, exps, &cnt);

        /* Must be square‑free composite (at least two distinct primes,
           each with exponent 1). */
        bool ok = (cnt >= 2);
        for (size_t i = 0; ok && i < cnt; ++i)
            if (exps[i] != 1) ok = false;

        if (ok) {
            /* Sort the prime factors in ascending order. */
            for (size_t i = 0; i + 1 < cnt; ++i)
                for (size_t j = i + 1; j < cnt; ++j)
                    if (primes[j] < primes[i]) {
                        uint64_t t = primes[i]; primes[i] = primes[j]; primes[j] = t;
                        t = exps[i]; exps[i] = exps[j]; exps[j] = t;
                    }

            /* Concatenate the sorted primes to form a decimal number. */
            seq_t cat = 0;
            for (size_t i = 0; i < cnt; ++i) {
                /* Compute 10^(number of decimal digits of the prime). */
                seq_t base = 1;
                uint64_t d = digit_count(primes[i]);
                for (uint64_t k = 0; k < d; ++k) base *= 10;

                cat = cat * base + (seq_t)primes[i];
            }

            /* Check whether the concatenated number is a palindrome. */
            if (is_palindrome(cat, 10)) {
                ++found;
                if (found == n) return (seq_t)cand;
            }
        }

        ++cand;
    }

    /* unreachable */
    return 0;
}
