/*
 * A096883  Expansion of (1+10x)/(1-101x^2).
 * https://oeis.org/A096883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 101^(⌊n/2⌋) if n even a(n) = 10*101^(⌊n/2⌋) if n odd The
 *            sequence is the coefficient expansion of (1+10x)/(1-101x^2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Power cache for 101^k */
    static seq_t *pow_cache = NULL; /* dynamic array of sequence values */
    static idx_t pow_cap = 0;       /* capacity of pow_cache */
    static idx_t pow_max = 0;       /* largest k for which pow_cache[k] is defined */

    if (pow_cache == NULL) {
        pow_cap = 1;
        pow_cache = (seq_t *)realloc(NULL, pow_cap * sizeof(seq_t));
        if (!pow_cache) return 0;          /* allocation failed; return 0 as fallback */
        pow_cache[0] = (seq_t)1;           /* 101^0 = 1 */
        pow_max = 0;
    }

    /* exponent k = floor(n/2) */
    idx_t k = n >> 1;                      /* integer division by 2 */

    /* enlarge power cache if needed */
    if (k > pow_max) {
        /* ensure capacity */
        if (k + 1 > pow_cap) {
            idx_t new_cap = pow_cap * 2;
            if (new_cap <= k) new_cap = k + 1;
            seq_t *temp = (seq_t *)realloc(pow_cache, new_cap * sizeof(seq_t));
            if (!temp) return 0;           /* allocation failed; return 0 */
            pow_cache = temp;
            pow_cap = new_cap;
        }
        /* compute missing powers */
        for (idx_t i = pow_max + 1; i <= k; ++i) {
            pow_cache[i] = pow_cache[i - 1] * (seq_t)101;
        }
        pow_max = k;
    }

    /* return the term based on parity of n */
    if ((n & 1) == 0) {        /* even */
        return pow_cache[k];
    } else {                   /* odd */
        return pow_cache[k] * (seq_t)10;
    }
}
