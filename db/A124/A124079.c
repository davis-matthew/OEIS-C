/*
 * A124079  a(n) = H(2n+1)*(2n+1)!/n!, where H(n) = Sum_{k=1..n} 1/k, the n-th
 *          harmonic number.
 * https://oeis.org/A124079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = H(2n+1)*(2n+1)!/n!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache_buf   = NULL;   /* a(n) values */
static seq_t *prod_buf    = NULL;   /* P_n = (n+1)…(2n+1) */
static idx_t  buf_size    = 0;      /* allocated size of buffers */

seq_t a(idx_t n)
{
    /* Resize buffers if needed */
    if (n >= buf_size) {
        idx_t new_size = n + 1;
        cache_buf = (seq_t *)realloc(cache_buf, new_size * sizeof(seq_t));
        prod_buf  = (seq_t *)realloc(prod_buf,  new_size * sizeof(seq_t));

        /* Fill missing entries */
        for (idx_t i = buf_size; i <= n; ++i) {
            if (i == 0) {
                cache_buf[0] = (seq_t)1;  /* a(0) */
                prod_buf[0]  = (seq_t)1;  /* P_0 = 1 */
                continue;
            }

            seq_t prevP = prod_buf[i-1];
            seq_t prevA = cache_buf[i-1];
            seq_t two_i = (seq_t)((idx_t)2 * i);          /* 2*i   */
            seq_t two_i_plus1 = (seq_t)((idx_t)2 * i + 1);/* 2*i+1 */

            /* P_i = P_{i-1} * (2i) * (2i+1) / i */
            seq_t prod = (prevP * two_i) * two_i_plus1 / i;
            prod_buf[i] = prod;

            /* a(i) = 2*(2i+1)*a_{i-1} + (2i+1)*P_{i-1}/i + 2*P_{i-1} */
            seq_t term1 = prevA * ((seq_t)2 * two_i_plus1);
            seq_t term2 = (prevP * two_i_plus1) / i;
            seq_t term3 = prevP * (seq_t)2;
            cache_buf[i] = term1 + term2 + term3;
        }
        buf_size = new_size;
    }

    return cache_buf[n];
}
