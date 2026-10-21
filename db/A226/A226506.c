/*
 * A226506  a(n) = B(n+2)-3*B(n+1)+B(n), where B(i) are the Bell numbers
 *          A000110.
 * https://oeis.org/A226506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = B(n+2) - 3*B(n+1) + B(n), where B(i) are Bell numbers
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 23, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function caches Bell numbers in a static array that is expanded as needed. */

#include <stdlib.h>

/* External linkage as required by the build. */
seq_t a(idx_t n)
{
    /* Cache of Bell numbers.  B[0] = 1 is always known. */
    static seq_t *B = NULL;
    static idx_t B_len = 0;          /* number of Bell numbers currently stored */

    /* Desired maximum index to compute (n+2). */
    idx_t need = n + 2;

    /* Initial allocation if this is the first call. */
    if (B == NULL) {
        B_len = 1;

        B = (seq_t *)malloc(sizeof(seq_t) * B_len);
        if (B == NULL)
            return (seq_t)0;          /* Should never happen in normal build. */

        B[0] = (seq_t)1;              /* B(0) = 1 */
    }

    /* Expand the cache if required. */
    if (B_len <= need) {
        /* Allocate space for indices 0 .. need (inclusive). */
        idx_t new_len = need + 1;
        seq_t *old = B;
        B = (seq_t *)realloc(B, sizeof(seq_t) * new_len);
        if (B == NULL) {
            /* Allocation failure – restore old pointer and return 0. */
            B = old;
            return (seq_t)0;
        }

        /* Compute Bell numbers from B_len to need. */
        for (idx_t i = B_len; i <= need; ++i) {
            seq_t sum   = (seq_t)0;
            seq_t binom = (seq_t)1;   /* C(i-1,0) */

            for (idx_t k = 0; k < i; ++k) {
                sum += binom * B[k];

                /* Update binom to C(i-1, k+1). The division is exact. */
                seq_t mult = (seq_t)( (i - 1) - k );
                binom = (binom * mult) / (k + 1);
            }
            B[i] = sum;
        }
        B_len = new_len;
    }

    /* Compute a(n) using the cached Bell numbers. */
    seq_t result = B[n + 2];
    result -= B[n + 1] * (seq_t)3;
    result += B[n];

    return result;
}
