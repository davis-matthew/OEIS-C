/*
 * A071052  Number of 0's in n-th row of triangle in A071036 (cellular
 *          automaton "Rule 150").
 * https://oeis.org/A071052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 0's in the n‑th row of the Rule 150
 *            cellular‑automaton triangle
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static idx_t    max_n   = 0;      /* greatest n computed so far   */
    static seq_t  *zero_cnt = NULL;   /* a[k] for k <= max_n          */
    static uint8_t *row_cur = NULL;   /* current row bits             */
    static uint8_t *row_nxt = NULL;   /* buffer for next row          */
    static size_t  cur_len = 0;       /* length of row_cur            */

    /* ---- first call: allocate and initialise ---- */
    if (!zero_cnt) {
        zero_cnt = (seq_t *)malloc(sizeof(seq_t));
        if (!zero_cnt) abort();          /* allocation failure */
        zero_cnt[0] = 0;                /* a(0) = 0 */

        /* first row has length 1, value 1 at centre */
        cur_len  = 1;
        row_cur  = (uint8_t *)calloc(3, sizeof(uint8_t));   /* buffer big enough for the first few rows */
        row_cur[0] = 1;                   /* row 0: [1] */
        row_nxt  = (uint8_t *)calloc(3, sizeof(uint8_t));
        max_n    = 0;
    }

    /* ---- compute rows up to requested n ---- */
    while (max_n < n) {
        /* Length of next row */
        size_t new_len = cur_len + 2;     /* 2*(max_n+1)+1 */

        /* Ensure buffer for the new row is large enough */
        uint8_t *tmp = (uint8_t *)realloc(row_nxt, new_len * sizeof(uint8_t));
        if (!tmp) abort();               /* allocation failure */
        row_nxt = tmp;

        /* Count ones while generating the new row */
        seq_t ones = 0;
        for (size_t i = 0; i < new_len; ++i) {
            uint8_t left  = (i >= 2) ? row_cur[i-2] : 0;
            uint8_t mid   = (i >= 1 && i-1 < cur_len) ? row_cur[i-1] : 0;
            uint8_t right = (i < cur_len) ? row_cur[i] : 0;
            uint8_t val   = left ^ mid ^ right;
            row_nxt[i] = val;
            ones += (seq_t)val;
        }

        /* Store number of zeros: zeros = new_len - ones */
        seq_t zeros = (seq_t)new_len - ones;
        seq_t *tmpz = (seq_t *)realloc(zero_cnt, sizeof(seq_t) * (max_n + 2));
        if (!tmpz) abort();
        zero_cnt = tmpz;
        zero_cnt[max_n + 1] = zeros;

        /* Prepare for next iteration */
        uint8_t *swap = row_cur;
        row_cur = row_nxt;
        row_nxt = swap;
        cur_len = new_len;
        ++max_n;
    }

    return zero_cnt[n];
}
