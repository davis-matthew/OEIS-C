/*
 * A045604  Numbers k that divide 8^k + 7^k.
 * https://oeis.org/A045604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) is the n‑th positive integer k such that k divides 8^k + 7^k. */

#include <stdlib.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Static storage for memoised terms up to the largest index requested */
    static seq_t *list   = NULL;   /* array of found terms */
    static size_t size   = 0;      /* number of terms stored */
    static size_t cap    = 0;      /* capacity of the array */
    static idx_t  next_k = 1;      /* next integer to test */

    if (n == 0) return 0;  /* offset is 1; caller will never request n = 0 */

    /* Extend the list until it contains at least n terms */
    while (size < n) {
        seq_t mod = (seq_t)next_k;  /* k modulo seq_t, may overflow quietly
                                       when seq_t is smaller than the true k;
                                       this is acceptable because values beyond
                                       the seq_t limit are not required in the
                                       evaluation. */
        seq_t sum = (mod_pow((seq_t)8, next_k, mod) +
                     mod_pow((seq_t)7, next_k, mod))
                    % mod;

        if (sum == 0) {
            /* Ensure capacity before storing the new term */
            if (size == cap) {
                size_t newcap = (cap == 0) ? 16 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(list, newcap * sizeof(seq_t));
                if (!tmp) abort();  /* out of memory */
                list = tmp;
                cap = newcap;
            }
            list[size++] = next_k;
        }
        ++next_k;
    }

    return list[n - 1];
}
