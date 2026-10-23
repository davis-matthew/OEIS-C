/*
 * A124965  Odd values of 2^n mod n corresponding to the n's given in A015911.
 * https://oeis.org/A124965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A124965
 * A124965: Odd values of 2^n mod n corresponding to the n's given in A015911.
 * This implementation enumerates odd integers k for which 2^k mod k is odd,
 * stores the odd remainders, and returns the n‑th such remainder.
 * The sequence is generated on demand; results are cached to support arbitrary
 * call order.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n) {
    static seq_t *cache  = NULL;   /* array of discovered odd remainders */
    static size_t  cap   = 0;       /* current allocation size   */
    static size_t  count = 0;       /* number of terms stored    */
    static idx_t   cur   = 1;       /* candidate integer k       */

    /* Generate terms up to the requested index n */
    while (count < n) {
        /* Move to the next odd candidate */
        cur += 1;                   /* skip current odd */
        if ((cur & 1) == 0)         /* skip even numbers */
            cur += 1;

        /* r = 2^cur (mod cur) */
        seq_t r = mod_pow((seq_t)2, (uint64_t)cur, (seq_t)cur);

        /* If the remainder is odd, store it */
        if ((r & (seq_t)1) != 0) {
            if (count == cap) {               /* grow cache if needed */
                size_t newcap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache,
                                               newcap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cap   = newcap;
            }
            cache[count++] = r;
        }
    }
    return cache[n - 1];
}
