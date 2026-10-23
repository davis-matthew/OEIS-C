/*
 * A049973  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m =
 *          2^(p+1) + 2 - n and p is the unique integer such that 2^p < n - 1
 *          <= 2^(p+1), starting with a(1) = 1 and a(2) = a(3) = 3.
 * https://oeis.org/A049973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=3, a(3)=3; for n>=4, let p = floor_log2(n-2)
 *            (unique integer such that 2^p < n-1 <= 2^(p+1)); then m =
 *            2^(p+1) + 2 - n a(n) = (a(1)+...+a(n-1)) + a(m)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   floor_log2_uint64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/floor_log2_uint64.h"
#include "../utils/pow2.h"

static seq_t *a_arr   = NULL;   /* a values, 1‑based; a_arr[0] unused */
static seq_t *pref    = NULL;   /* cumulative sums, pref[0]=0 */
static size_t cap     = 0;      /* allocated capacity (array size) */
static idx_t  last    = 0;      /* highest index that has been computed */

/* Ensure at least 'needed' positions in the arrays. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cap)
        return;

    size_t newcap = cap ? cap : 1;
    while (newcap < needed)
        newcap <<= 1;            /* double until sufficient */

    seq_t *tmp = (seq_t *)realloc(a_arr, newcap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    a_arr = tmp;

    tmp = (seq_t *)realloc(pref, newcap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    pref = tmp;

    cap = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0)                  /* defensive, offset is 1 */
        return (seq_t)0;

    ensure_capacity((size_t)n + 1);
    pref[0] = (seq_t)0;           /* keep definition explicit */

    if (last >= n)                /* already computed */
        return a_arr[n];

    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 1) {
            a_arr[1] = (seq_t)1;
            pref[1] = pref[0] + a_arr[1];
        } else if (i == 2 || i == 3) {
            a_arr[i] = (seq_t)3;
            pref[i] = pref[i-1] + a_arr[i];
        } else {
            /* use p = floor_log2(i-2) to satisfy 2^p < i-1 <= 2^(p+1) */
            idx_t p = floor_log2_uint64(i - 2);
            idx_t pow2p1 = (idx_t)pow2(p + 1);   /* 2^(p+1) */
            idx_t m = pow2p1 + (idx_t)2 - i;     /* m = 2^(p+1)+2-i */

            seq_t val = pref[i - 1] + a_arr[m];
            a_arr[i] = val;
            pref[i] = pref[i - 1] + val;
        }
    }

    last = n;
    return a_arr[n];
}
