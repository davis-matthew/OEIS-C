/*
 * A108103  Fixed point of the square of the morphism: 1->3, 2->1, 3->121,
 *          starting with 1.
 * https://oeis.org/A108103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th letter of the fixed point of the square of the
 *            * morphism σ defined by σ(1)=3, σ(2)=1, σ(3)=121, i.e. τ where *
 *            τ(1)=121, τ(2)=3, τ(3)=313
 * Range:     u8 n <= 1200, u16 n <= 1200, u32 n <= 1200, u64 n <= 1200,
 *            u128 n <= 1200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const uint8_t expmap[4][4] = {
    {0, 0, 0, 0},          /* unused index 0 */
    {1, 2, 1, 0},          /* τ(1) = 121 */
    {3, 0, 0, 0},          /* τ(2) = 3  */
    {3, 1, 3, 0}           /* τ(3) = 313 */
};

static const uint8_t explen[4] = {0, 3, 1, 3};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n=0, return 0 */

    /* Start with the seed word "1". */
    uint8_t *cur  = (uint8_t *)malloc(1);
    cur[0]       = 1;
    idx_t cur_len = 1;

    while (cur_len < n) {
        /* Compute length of the next expansion. */
        idx_t new_len = 0;
        for (idx_t i = 0; i < cur_len; ++i)
            new_len += explen[cur[i]];

        /* Perform the expansion. */
        uint8_t *next = (uint8_t *)malloc((size_t)new_len * sizeof(uint8_t));
        idx_t pos = 0;
        for (idx_t i = 0; i < cur_len; ++i) {
            uint8_t c = cur[i];
            const uint8_t *exp = expmap[c];
            for (uint8_t k = 0; k < explen[c]; ++k)
                next[pos++] = exp[k];
        }

        free(cur);
        cur     = next;
        cur_len = new_len;
    }

    uint8_t result = cur[n - 1]; /* zero‑based index */
    free(cur);
    return (seq_t)result;
}
