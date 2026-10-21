/*
 * A203161  (n-1)-st elementary symmetric function of the first n terms of
 *          (3,1,2,3,1,2,3,1,2,...).
 * https://oeis.org/A203161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A203161 – (n‑1)-st elementary symmetric function of 3,1,2 repeating.
   Formula:
     Let k = floor(n/3) and r = n mod 3.
     Let N = 11*k + 2*(r>=1) + 6*(r>=2).
     Then
        if r == 0 : a(n) = 6^(k-1) * N          (k ≥ 1)
        else      : a(n) = 6^k * N / 2          (k ≥ 0)
   The values are computed using dynamic caching of powers of 6.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset starts at 1; harmless guard */
        return 0;

    /* cache of powers of 6: pow6[i] = 6^i  (i ≥ 0) */
    static seq_t *pow6 = NULL;
    static idx_t  pow6_len = 0;   /* current number of stored powers */

    if (pow6_len == 0) {                /* first call */
        pow6 = (seq_t *)malloc(sizeof(seq_t));
        if (!pow6) abort();
        pow6[0] = 1;                    /* 6^0 = 1 */
        pow6_len = 1;
    }

    /* Needed exponent depends on r */
    idx_t k = n / 3;
    idx_t r = n % 3;
    idx_t need_exp = (r == 0) ? k - 1 : k;   /* highest 6^i required */

    if (need_exp + 1 > pow6_len) {          /* expand cache as necessary */
        size_t new_len = need_exp + 1;
        seq_t *tmp = (seq_t *)realloc(pow6, new_len * sizeof(seq_t));
        if (!tmp) abort();
        pow6 = tmp;
        for (idx_t i = pow6_len; i < new_len; ++i)
            pow6[i] = pow6[i - 1] * 6;
        pow6_len = new_len;
    }

    /* Compute N = 11*k + 2*(r≥1) + 6*(r≥2) */
    seq_t N = (seq_t)(11 * k);
    if (r >= 1) N += 2;
    if (r >= 2) N += 6;

    seq_t result;
    if (r == 0) {   /* a(n) = 6^(k-1) * N */
        result = pow6[k - 1] * N;
    } else {        /* a(n) = 6^k * N / 2 */
        result = (pow6[k] * N) / 2;
    }

    return result;
}
