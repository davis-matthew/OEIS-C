/*
 * A225873  Squares that become prime when their most significant (or
 *          leftmost) digit is removed.
 * https://oeis.org/A225873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 40, u32 n <= 3589, u64 n <= 8585,
 *            u128 n <= 8585, big any n
 * Helpers:   digit_len, is_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generate sequence A225873: 
 * a(n) is the square of the n-th positive integer m such that
 * removing the most significant decimal digit of m² results in a prime number.
 * The sequence is computed lazily and memoized for efficiency.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_len.h"
#include "../utils/pow10.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* Static cache */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t sz    = 0;
    static idx_t  m_last = 0;           /* last m checked */

    if (n == 0) return 0;               /* defensive guard */

    /* Generate terms until we reach the desired index */
    while (sz < n) {
        m_last++;
        seq_t m = (seq_t)m_last;
        seq_t sq = m * m;

        /* Remove most significant decimal digit */
        int  dlen   = digit_len(sq);            /* number of digits of sq   */
        seq_t ten   = pow10((idx_t)(dlen - 1)); /* 10^(dlen-1)              */
        seq_t rem   = sq % ten;                 /* remainder after removal   */

        /* Test for primality (ignore 0 and 1, which are not prime) */
        if (rem > 1 && is_prime((idx_t)rem)) {
            if (sz == cap) {                   /* grow cache if needed   */
                cap = cap ? cap * 2 : 8;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[sz++] = sq;                    /* store square           */
        }
    }

    return cache[n - 1];
}
