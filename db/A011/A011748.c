/*
 * A011748  Expansion of (1 + x^2 + x^4)/(1 + x + x^2 + x^4 + x^5) mod 2.
 * https://oeis.org/A011748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in (1 + x^2 + x^4)/(1 + x + x^2 + x^4
 *            + x^5) over GF(2). Equivalently a recurrence: a(n) = n_n +
 *            a(n-1) + a(n-2) + a(n-4) + a(n-5) (mod 2), where n_n = 1 for
 *            n=0,2,4 and 0 otherwise
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *buf = NULL;
    static idx_t  computed = 0;   /* numbers already computed: 0 .. computed-1 */

    /* grow buffer if needed */
    if (n + 1 > computed) {
        idx_t new_size = n + 1;
        /* shrink or enlarge as necessary */
        seq_t *tmp = (seq_t *)realloc(buf, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure – should never happen */
        buf = tmp;

        /* compute from previous size up to n */
        for (idx_t i = computed; i <= n; ++i) {
            seq_t ncoeff = ((i == (idx_t)0) ||
                            (i == (idx_t)2) ||
                            (i == (idx_t)4)) ? (seq_t)1 : (seq_t)0;
            seq_t sum = ncoeff;
            if (i >= (idx_t)1) sum ^= buf[i-1];
            if (i >= (idx_t)2) sum ^= buf[i-2];
            if (i >= (idx_t)4) sum ^= buf[i-4];
            if (i >= (idx_t)5) sum ^= buf[i-5];
            buf[i] = sum;
        }
        computed = new_size;
    }
    return buf[n];
}
