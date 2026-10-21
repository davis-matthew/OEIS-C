/*
 * A050056  a(n) = a(n-1) + a(m) for n >= 4, where m = 2*n - 3 - 2^(p+1) and p
 *          is the unique integer such that 2^p < n - 1 <= 2^(p+1), starting
 *          with a(1) = 1, a(2) = 3, and a(3) = 1.
 * https://oeis.org/A050056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 59, u32 n <= 197, u64 n <= 771,
 *            u128 n <= 3649, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*********************************************************************
 *  OEIS A050056:  a(1) = 1, a(2) = 3, a(3) = 1.
 *  For n ≥ 4,
 *      a(n) = a(n-1) + a(m),
 *  where m = 2*n - 3 - 2^(p+1) and p is the unique integer
 *      2^p < n-1 ≤ 2^(p+1).
 *  The array is memoised so calls may arrive in any order.
 *********************************************************************/
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The function must be thread‑local only to the static data */
    static seq_t *arr = NULL;   /* 1‑based indexing, arr[0] unused */
    static size_t arr_len = 0;
    static idx_t last = 3;      /* last computed index */

    if (n < 1) return 0;        /* Outside the offset; undefined behaviour */

    if (arr == NULL) {
        /* Allocate a small initial buffer and seed known values */
        arr_len = 16;
        arr = (seq_t *)calloc(arr_len, sizeof(seq_t));
        if (!arr) exit(EXIT_FAILURE);
        arr[1] = (seq_t)1;
        arr[2] = (seq_t)3;
        arr[3] = (seq_t)1;
        last = 3;
    }

    /* Grow array if necessary (1‑based, so size must be >= n+1) */
    if ((size_t)n >= arr_len) {
        size_t new_len = arr_len;
        while (new_len <= (size_t)n) new_len <<= 1;
        seq_t *tmp = (seq_t *)realloc(arr, new_len * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        /* Newly allocated elements get default constructed value (e.g. 0) */
        for (size_t i = arr_len; i < new_len; ++i) tmp[i] = (seq_t)0;
        arr = tmp;
        arr_len = new_len;
    }

    /* Return cached result if already computed */
    if ((idx_t)n <= last) return arr[n];

    /* Compute terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* i >= 4 in this loop because last >= 3 */
        /* Compute p: largest integer with 2^p < i-1 ≤ 2^(p+1) */
        /* Since i ≥ 4, i-2 > 0 */
        unsigned int bl = 64 - __builtin_clzll((unsigned long long)(i - 2));
        /* bl = floor_log2(i-2) + 1  ->  p+1 = bl  ->  2^(p+1) = 1 << bl  */
        seq_t pow = (seq_t)1 << bl;          /* 2^(p+1) */
        seq_t m = ((seq_t)i) * (seq_t)2 - (seq_t)3 - pow;
        arr[i] = arr[i - 1] + arr[(idx_t)m];
    }
    last = n;
    return arr[n];
}
