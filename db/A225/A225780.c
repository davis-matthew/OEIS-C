/*
 * A225780  Numbers such that both n and its reversal are divisible by sum of
 *          digits of n.
 * https://oeis.org/A225780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 836, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) of OEIS A225780: the n-th integer n such that n
   and its decimal reversal are both divisible by the sum of its digits. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/rev.h"

seq_t a(idx_t n)
{
    static seq_t  *cache = NULL;   /* cache of found terms    */
    static size_t cached = 0;      /* how many terms stored  */
    static size_t cap    = 0;      /* cache capacity          */
    static idx_t  candidate = 1;   /* next integer to test    */

    /* Return the cached value if already computed. */
    if (n > 0 && n <= cached) {
        return cache[n - 1];
    }

    /* Initialise cache lazily */
    if (cap == 0) {
        cap   = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Find terms until we have at least n of them. */
    while (cached < n) {
        seq_t num = (seq_t)candidate;
        seq_t sum = digit_sum(num);
        if (sum != 0) {
            seq_t rev_val = rev(candidate);      /* reverse of candidate */
            if ((num % sum == 0) && (rev_val % sum == 0)) {
                /* store term */
                if (cached == cap) {
                    size_t new_cap = cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort(); /* allocation failure */
                    cache = tmp;
                    cap   = new_cap;
                }
                cache[cached++] = num;
            }
        }
        candidate++;
    }

    return cache[n - 1];
}
