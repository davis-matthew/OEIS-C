/*
 * A086379  Numbers such that both 1 + sum of digits and 1 + product of digits
 *          are prime.
 * https://oeis.org/A086379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A086379: numbers such that 1 + (sum of digits) and 1 + (product of digits)
   are prime.
   The function searches the positive integers in order, tests the two
   primality conditions, stores found values in a static cache, and
   returns the n‑th element (a(1) = 1). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* undefined for n == 0 – the behavior below the OEIS offset is not
       required to be correct, but we guard against it. */
    if (n == 0) {
        return 0;
    }

    /* Static cache of found terms */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;

    /* The next integer to test */
    static seq_t next_check  = 1;

    /* Keep finding terms until we have at least n of them. */
    while (cache_len < (size_t)n)
    {
        seq_t m    = next_check;
        seq_t sum  = 0;
        seq_t prod = 1;

        /* If m == 0 (should not happen for n >= 1) handle separately */
        if (m == 0) {
            sum = 0;
            prod = 0;
        } else {
            /* digit extraction in base 10 */
            while (m > 0)
            {
                seq_t digit = m % ((seq_t)10);
                sum += digit;
                prod *= digit;
                m /= ((seq_t)10);
            }
        }

        bool ok = false;
        /* Check 1 + sum of digits is prime, then 1 + product of digits. */
        if (is_prime_big(sum + ((seq_t)1))) {
            ok = is_prime_big(prod + ((seq_t)1));
        }

        if (ok)
        {
            /* Grow cache if necessary */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failure – abort the program safely. */
                    /* Avoid a crash in the older C standard; C++ would throw. */
                    exit(1);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_check;
        }

        ++next_check;
    }

    return cache[n - 1];
}
