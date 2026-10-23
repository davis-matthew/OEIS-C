/*
 * A025089  a(n) = s(1)*s(n) + s(2)*s(n-1) + ... + s(k)*s(n-k+1), where k =
 *          [n/2], s = (Lucas numbers).
 * https://oeis.org/A025089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=1}^{⌊n/2⌋} L_i * L_{n-i+1} where (L_0, L_1) = (2, 1)
 *            and L_k = L_{k-1} + L_{k-2} (Lucas numbers)
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static arrays for Lucas numbers (L[1..]) and a[1..] */
    static seq_t *lucas = NULL;   /* lucas[i] = L_i, i ≥ 1 */
    static seq_t *res   = NULL;   /* res[i]   = a(i)   */
    static idx_t  max_n = 0;      /* largest n that has been computed */
    static idx_t  size  = 0;      /* allocated array size (indexable up to size-1) */

    /* enlarge storage if required */
    if (n + 1 > size) {
        idx_t newsize = (n + 1) * 2;          /* double to reduce reallocs */
        if (size == 0) {
            /* first allocation: also set initial Lua values 1 and 3 */
            lucas = (seq_t *)realloc(NULL, (newsize) * sizeof(seq_t));
            res   = (seq_t *)realloc(NULL, (newsize) * sizeof(seq_t));
            size  = newsize;
            if (size > 2) {                   /* fill L_1 = 1, L_2 = 3 */
                lucas[1] = (seq_t)1;
                lucas[2] = (seq_t)3;
            }
        } else {
            lucas = (seq_t *)realloc(lucas, newsize * sizeof(seq_t));
            res   = (seq_t *)realloc(res,   newsize * sizeof(seq_t));
            size  = newsize;
        }
    }

    /* compute Lucas numbers up to n if necessary */
    if (max_n < n) {
        if (max_n < 2) {
            /* ensure base cases are set */
            lucas[1] = (seq_t)1;
            lucas[2] = (seq_t)3;
        }
        for (idx_t i = (max_n > 2 ? max_n : 2) + 1; i <= n; ++i) {
            lucas[i] = lucas[i - 1] + lucas[i - 2];
        }
    }

    /* compute a(i) for each missing i up to n */
    for (idx_t i = (max_n < 1 ? 1 : max_n + 1); i <= n; ++i) {
        seq_t sum = 0;
        idx_t k   = i >> 1;            /* floor(i/2) */
        for (idx_t j = 1; j <= k; ++j) {
            sum += lucas[j] * lucas[i - j + 1];
        }
        res[i] = sum;
    }

    max_n = n;              /* update the largest computed index */
    return res[n];
}
