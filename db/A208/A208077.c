/*
 * A208077  Number of n X 7 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 0 and 0 1 0 vertically.
 * https://oeis.org/A208077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: dynamic programming over rows with vertical constraints */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Ensure that the static data are initialized only once. */
    static bool initialized = false;
    static int M;                    /* number of allowed row patterns */
    static int *allowedRows;         /* indices of allowed rows   */
    static int NULL_IDX;             /* index representing a null row */
    static int states;               /* M + 1                     */
    static size_t pairCount;         /* states * states            */
    static size_t *next_offsets;     /* start index for each prev  */
    static size_t *next_counts;      /* number of next rows         */
    static int *next_values;         /* concatenated lists          */
    static seq_t *dp_cur;            /* current DP table          */
    static seq_t *dp_next;           /* next DP table             */

    if (!initialized)
    {
        /* 1. Build list of allowed rows (no "000" or "001" horizontally). */
        int rows[128];
        M = 0;
        for (int mask = 0; mask < (1 << 7); ++mask)
        {
            bool ok = true;
            for (int j = 0; j <= 4; ++j)
            {
                int sub = (mask >> j) & 7;
                if (sub == 0 || sub == 1) { ok = false; break; }
            }
            if (ok) rows[M++] = mask;
        }
        allowedRows = (int*)malloc(M * sizeof(int));
        for (int i = 0; i < M; ++i) allowedRows[i] = rows[i];

        NULL_IDX = M;                /* null row index */
        states = M + 1;
        pairCount = (size_t)states * states;

        /* 2. Pre‑compute transition lists for each previous row. */
        next_offsets = (size_t*)malloc(states * sizeof(size_t));
        next_counts  = (size_t*)malloc(states * sizeof(size_t));

        size_t total_next = 0;
        for (int prev = 0; prev < states; ++prev)
        {
            if (prev == NULL_IDX) {
                next_counts[prev] = (size_t)M;
            } else {
                int amask = allowedRows[prev];
                int required = (~amask) & 0x7F;   /* bits that must be 1 in c */
                size_t cnt = 0;
                for (int c = 0; c < M; ++c)
                    if ((allowedRows[c] & required) == required) ++cnt;
                next_counts[prev] = cnt;
            }
            next_offsets[prev] = total_next;
            total_next += next_counts[prev];
        }

        next_values = (int*)malloc(total_next * sizeof(int));
        for (int prev = 0; prev < states; ++prev)
        {
            size_t offset = next_offsets[prev];
            size_t cnt = next_counts[prev];
            if (prev == NULL_IDX) {
                for (size_t i = 0; i < cnt; ++i) next_values[offset + i] = (int)i;
            } else {
                int amask = allowedRows[prev];
                int required = (~amask) & 0x7F;
                size_t idx = 0;
                for (int c = 0; c < M; ++c)
                    if ((allowedRows[c] & required) == required)
                        next_values[offset + idx++] = c;
            }
        }

        /* 3. Allocate DP tables. */
        dp_cur  = (seq_t*)calloc(pairCount, sizeof(seq_t));
        dp_next = (seq_t*)calloc(pairCount, sizeof(seq_t));

        initialized = true;
    }

    /* 4. Special case: sequence is defined from offset 1. */
    if (n == 0) return 0;

    /* 5. Reset DP tables. */
    for (size_t i = 0; i < pairCount; ++i) dp_cur[i] = 0;
    /* initial state: no previous rows. */
    dp_cur[(size_t)NULL_IDX * states + NULL_IDX] = 1;

    /* 6. Iterate over rows. */
    for (idx_t row = 0; row < n; ++row)
    {
        /* clear next table */
        for (size_t i = 0; i < pairCount; ++i) dp_next[i] = 0;

        /* iterate over all states (prev, curr). */
        for (int prev = 0; prev < states; ++prev)
        {
            for (int curr = 0; curr < states; ++curr)
            {
                size_t id = (size_t)prev * states + curr;
                seq_t val = dp_cur[id];
                if (!val) continue;

                size_t offset = next_offsets[prev];
                size_t cnt    = next_counts[prev];
                for (size_t k = 0; k < cnt; ++k)
                {
                    int next = next_values[offset + k];
                    size_t next_id = (size_t)curr * states + next;
                    dp_next[next_id] += val;
                }
            }
        }
        /* swap tables */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* 7. Sum all entries to get the answer. */
    seq_t ans = 0;
    for (size_t i = 0; i < pairCount; ++i)
        ans += dp_cur[i];

    return ans;
}
