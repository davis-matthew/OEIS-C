/*
 * A049894  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m = n -
 *          1 - 2^p and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = a(2) = 1 and a(3) = 3.
 * https://oeis.org/A049894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 a(2) = 1 a(3) = 3 a(n) = (∑_{k=1}^{n-1} a(k)) −
 *            a(n‑1‑2^p) for n ≥ 4, where p is the unique integer with 2^p <
 *            n‑1 ≤ 2^(p+1)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static memoization storage */
    static seq_t *a_arr = NULL;      /* a[1..max_n]          */
    static seq_t *prefix = NULL;    /* prefix sums to i     */
    static size_t   cap_a = 0;      /* capacity of a_arr    */
    static size_t   cap_p = 0;      /* capacity of prefix   */
    static idx_t    max_n = 0;      /* largest n computed   */

    /* Reallocate storage if necessary */
    if (cap_a < (size_t)(n + 1)) {
        seq_t *tmp = (seq_t *)realloc(a_arr, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) abort();
        a_arr = tmp;
        cap_a = n + 1;
    }
    if (cap_p < (size_t)(n + 1)) {
        seq_t *tmp = (seq_t *)realloc(prefix, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) abort();
        prefix = tmp;
        cap_p = n + 1;
    }

    /* Compute missing values up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        if (i == 1 || i == 2) {
            a_arr[i] = (seq_t)1;
        } else if (i == 3) {
            a_arr[i] = (seq_t)3;
        } else {
            /* largest power of two strictly less than (i-1) */
            idx_t t = i - 1;
            idx_t bound = t - 1;
            idx_t power = 1;
            while (power <= bound) power <<= 1;
            power >>= 1;            /* power = 2^p           */

            idx_t m = i - 1 - power;    /* m = n-1-2^p            */
            a_arr[i] = prefix[i - 1] - a_arr[m];
        }

        /* Update prefix sum for next iterations */
        if (i == 1) {
            prefix[i] = a_arr[i];
        } else {
            prefix[i] = prefix[i - 1] + a_arr[i];
        }
    }

    /* Remember highest computed index */
    max_n = (n > max_n) ? n : max_n;
    return a_arr[n];
}
