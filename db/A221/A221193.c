/*
 * A221193  Simple continued fraction expansion of product {k >= 0} (1 - 2*(N
 *          - sqrt(N^2-1))^(4*k+3))/(1 - 2*(N - sqrt(N^2-1))^(4*k+1)) at N =
 *          3.
 * https://oeis.org/A221193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, cofr
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A221193: Simple continued fraction expansion of
 *  product{ k>=0 } (1-2*(3-2*sqrt(2))^(4k+3))/(1-2*(3-2*sqrt(2))^(4k+1))
 *
 *  a(4m    ) = 1
 *  a(4m-1) = t_{2m}   - 2
 *  a(4m+1) = t_{2m+1} / 2 - 2
 *
 *  where t_0 = 2,  t_1 = 6,
 *        t_k = 6*t_{k-1} - t_{k-2}   (k >= 2)
 *
 *  The array t_k is built once and extended lazily.  All arithmetic
 *  is performed in the type seq_t (unsigned integer or arbitrary‑precision).
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* even indices give 1 */
    if ((n & 1) == 0)
        return 1;

    /* odd indices – compute the required power p = (n+1)/2   */
    idx_t p = (n + 1) / 2;
    static seq_t *t_arr = NULL;     /* sequence t_k */
    static size_t t_len = 0;        /* number of elements stored */

    /* ensure t_k up to index p is available */
    if (t_len == 0) {                        /* first call */
        size_t init_len = (size_t)p + 1;      /* at least 2 */
        t_arr = (seq_t *)malloc(init_len * sizeof(seq_t));
        t_arr[0] = 2;
        t_arr[1] = 6;
        for (size_t i = 2; i < init_len; ++i)
            t_arr[i] = 6 * t_arr[i - 1] - t_arr[i - 2];
        t_len = init_len;
    } else if (t_len <= (size_t)p) {         /* extend */
        size_t new_len = (size_t)p + 1;
        t_arr = (seq_t *)realloc(t_arr, new_len * sizeof(seq_t));
        for (size_t i = t_len; i < new_len; ++i)
            t_arr[i] = 6 * t_arr[i - 1] - t_arr[i - 2];
        t_len = new_len;
    }

    /* compute the odd term from the appropriate t_k */
    if ((n & 3) == 3) {                      /* n ≡ 3 (mod 4) */
        return t_arr[p] - 2;
    } else {                                 /* n ≡ 1 (mod 4) */
        return (t_arr[p] / 2) - 2;
    }
}
