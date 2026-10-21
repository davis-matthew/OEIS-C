/*
 * A087449  a(n) = n * 4^(n-1) + (2*4^n + 1) / 3.
 * https://oeis.org/A087449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * 4^(n-1) + (2*4^n + 1) / 3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* Handle the first term explicitly */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* static cache of powers of 4: pow4[i] = 4^i */
    static seq_t *pow4 = NULL;
    static idx_t pow4_len = 0;      /* number of valid entries in pow4  */

    /* ensure pow4[i] is available up to index n */
    if (pow4_len <= n) {
        /* grow array */
        idx_t new_len = n + 1;
        seq_t *new_pow4 = (seq_t *)realloc(pow4, new_len * sizeof(seq_t));
        if (!new_pow4)
            abort();                 /* allocation failed – unlikely */
        pow4 = new_pow4;

        /* compute missing powers */
        if (pow4_len == 0) {
            pow4[0] = (seq_t)1;      /* 4^0 = 1 */
            pow4_len = 1;
        }
        for (idx_t i = pow4_len; i <= n; ++i)
            pow4[i] = pow4[i - 1] * (seq_t)4;
        pow4_len = new_len;
    }

    /* compute a(n) using cached powers of 4 */
    seq_t term1 = (seq_t)n * pow4[n - 1];
    seq_t term2 = ((seq_t)2 * pow4[n] + (seq_t)1) / (seq_t)3;
    return term1 + term2;
}
