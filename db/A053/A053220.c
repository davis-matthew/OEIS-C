/*
 * A053220  a(n) = (3*n-1) * 2^(n-2).
 * https://oeis.org/A053220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A053220
 *  Formula: a(n) = (3*n-1)*2^(n-2)   (offset 1)
 *
 *  The function returns the n-th term of the sequence.
 *  For n = 1 the formula is interpreted as a(1) = 1.
 *  The function uses a static table of powers of two
 *  to avoid recomputing them for successive calls.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(1) is special because (n-2) would be negative */
    if (n == 0) return (seq_t)0;       /* not used, but harmless */
    if (n == 1) return (seq_t)1;

    /* exponent = n-2 (n >= 2) */
    idx_t e = n - 2;

    /* static cache of powers of two: pow2[i] = 2^i */
    static seq_t *pow2 = NULL;
    static idx_t pow2_len = 0;

    if (pow2_len == 0) {
        pow2 = (seq_t*)malloc(sizeof(seq_t));
        pow2[0] = (seq_t)1;      /* 2^0 */
        pow2_len = 1;
    }

    /* extend the cache if necessary */
    while (pow2_len <= e) {
        seq_t next = pow2[pow2_len - 1];   /* previous power of two */
        next = next << 1;                  /* multiply by 2 */
        pow2 = (seq_t*)realloc(pow2, (pow2_len + 1) * sizeof(seq_t));
        pow2[pow2_len] = next;
        pow2_len++;
    }

    /* compute (3*n-1) * 2^(n-2) */
    seq_t mult = (seq_t)(3 * n - 1);
    return mult * pow2[e];
}
