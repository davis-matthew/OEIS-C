/*
 * A095781  Values of n for which A095777(n) is 12 (those terms which are
 *          expressible in decimal digits for bases 2 through 13, but not for
 *          base 14).
 * https://oeis.org/A095781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   has_all_dec_digits, has_nondec_digit_14
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute the OEIS sequence A095781
 *
 *  Definition (informal):
 *      a(n) is the n‑th positive integer m for which
 *      • the representation of m in every base b = 2,3,…,13 contains only
 *        decimal digits (0…9), and
 *      • the representation of m in base 14 contains at least one
 *        non‑decimal digit (10,11,12 or 13).
 *
 *  Implementation:
 *      A simple enumeration is sufficient for the required range
 *      (up to n ≈ 10 000).  The sequence values are stored in a
 *      static cache so that the function is well‑behaved in any call
 *      order.
 *
 *  Note:
 *      seq_t may be an arbitrary‑precision integer type; all operations
 *      are performed in that type, so no overflow occurs in the
 *      supported builds.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_all_dec_digits.h"
#include "../utils/has_nondec_digit_14.h"

/*--- the required public function ---------------------------------------*/
seq_t a(idx_t n)
{
    /* Static state retained between calls */
    static seq_t *cache      = NULL;
    static idx_t   cache_len = 0;
    static idx_t   cache_cap = 0;
    static idx_t   next_cand = 1;    /* smallest integer to test next */

    /* Ensure capacity for at least one more term */
    if (cache_len >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 4;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();      /* memory exhaustion */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Enumerate until we have found n terms */
    while (cache_len < n) {
        seq_t cand = next_cand;
        bool ok = true;

        /* Bases 2 through 13 must use only decimal digits */
        for (idx_t base = 2; base <= 13; ++base) {
            if (!has_all_dec_digits(cand, base)) {
                ok = false;
                break;
            }
        }

        /* Base 14 must contain a non‑decimal digit */
        if (ok) ok = has_nondec_digit_14(cand);

        if (ok) {
            /* Store the found term */
            cache[cache_len++] = cand;
        }

        ++next_cand;
    }

    return cache[n - 1];
}
