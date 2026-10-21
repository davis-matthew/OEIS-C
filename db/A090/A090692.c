/*
 * A090692  Expansion of 2*(x^2-9*x+15) / ((1+x)*(1-3*x+x^2)).
 * https://oeis.org/A090692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=30, a(1)=42, a(2)=146, a(n)=2*a(n-1)+2*a(n-2)-a(n-3)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 19, u64 n <= 42, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t cap  = 0;
    static size_t len  = 0;

    /* grow array if needed */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap << 1 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_buf = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_buf) return 0;    /* out of memory – unlikely for the test size */
        if (seq) {
            memcpy(new_buf, seq, len * sizeof(seq_t));
            free(seq);
        }
        seq = new_buf;
        cap = new_cap;
    }

    /* initialise first three terms if not yet done */
    if (len == 0) { seq[0] = 30; len = 1; }
    if (len == 1) { seq[1] = 42; len = 2; }
    if (len == 2) { seq[2] = 146; len = 3; }

    /* compute up to requested n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq[i] = (seq_t)((seq[i-1] * (seq_t)2) + (seq[i-2] * (seq_t)2) - seq[i-3]);
    }
    len = (size_t)n + 1;

    return seq[(size_t)n];
}
