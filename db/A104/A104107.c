/*
 * A104107  a(1) = 1, c(1)=1, if A(k) = sequence of first c(k) terms and if
 *          B(k) is A(k) with 0's and 1's exchanged, then A(k+1) = A(k)B(k)
 *          and c(k+1)= 2*c(k) if a(k) = 0, A(k+1) = A(k),0,B(k) and c(k+1)=
 *          2*c(k)+1 if a(k) = 1.
 * https://oeis.org/A104107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, c(1)=1;
   For k≥1, let A(k) be the first c(k) terms.
   Let B(k) be A(k) with 0↔1 swapped.
   If a(k)=0 then
       A(k+1)=A(k)B(k),  c(k+1)=2c(k);
   else (a(k)=1) then
       A(k+1)=A(k)0B(k), c(k+1)=2c(k)+1.
   This function constructs the sequence iteratively until the desired
   term is reached. */
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;   /* offset starts at 1, but guard */

    /* dynamic array of bits, stored as uint8_t (0 or 1) */
    size_t capacity = 16;
    uint8_t *seq = (uint8_t*)malloc(capacity);
    if (!seq) return (seq_t)0;  /* allocation failure, return zero */

    size_t len = 0;
    seq[len++] = 1;                    /* a(1) = 1 */
    idx_t k = 1;                       /* current position used as k */

    while (len < (size_t)n) {          /* build until we've got index n */
        uint8_t ak = seq[(size_t)k - 1];   /* value a(k) */

        size_t old_len = len;               /* length before expansion */
        /* compute new length needed */
        size_t add = old_len;               /* for B part */
        if (ak == 1) add += 1;              /* separator 0 for a(k)=1 */
        size_t new_len = old_len + add;

        /* ensure capacity */
        if (new_len > capacity) {
            size_t new_cap = capacity;
            while (new_cap < new_len) new_cap <<= 1;
            uint8_t *tmp = (uint8_t*)realloc(seq, new_cap);
            if (!tmp) { free(seq); return (seq_t)0; }
            seq = tmp;
            capacity = new_cap;
        }

        /* append according to rule */
        if (ak == 0) {
            for (size_t i = 0; i < old_len; ++i)
                seq[len++] = 1 - seq[i];    /* B(k) */
        } else {
            seq[len++] = 0;                /* separator 0 */
            for (size_t i = 0; i < old_len; ++i)
                seq[len++] = 1 - seq[i];    /* B(k) */
        }
        k++;  /* move to next k */
    }

    seq_t result = (seq_t)seq[(size_t)n - 1];
    free(seq);
    return result;
}
