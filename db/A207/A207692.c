/*
 * A207692  Number of nX7 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 1 1 1 vertically.
 * https://oeis.org/A207692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 16, u64 n <= 36, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence + DP for A207692: number of n×7 0/1 arrays avoiding
   horizontal 000,010 and vertical 001,111. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n) {
    /* Static tables and DP state */
    static bool tables_init = false;
    static idx_t M = 0, S = 0;               /* number of valid rows & states */
    static uint8_t valRows[128];             /* all 7‑bit rows that satisfy horizontal rule */
    static int8_t raw_to_idx[128];           /* mapping raw row -> index in valRows or -1 */
    static uint8_t *trans_counts = NULL;     /* number of outgoing transitions per state   */
    static uint16_t *transitions = NULL;     /* flat list of destination state indices     */
    static seq_t *dp = NULL, *ndp = NULL;    /* DP vectors for current and next step      */
    static seq_t *answers = NULL;            /* cache of a(n) for all computed n          */
    static size_t answers_size = 0;          /* allocated size of answers                */
    static idx_t curN = 0;                   /* number of rows currently represented in dp */
    static idx_t max_precomputed_n = 0;      /* highest n for which answers[] is valid    */

    /* One-time initialization of tables and base DP for n = 1,2 */
    if (!tables_init) {
        const uint8_t WIDTH = 7;
        const uint8_t MASK = (1U << WIDTH) - 1;   /* 0x7F */
        int i;

        /* Find all rows valid with respect to horizontal patterns 000 and 010 */
        for (i = 0; i < 128; ++i) raw_to_idx[i] = -1;
        M = 0;
        for (i = 0; i < 128; ++i) {
            bool ok = true;
            for (int pos = 0; pos <= WIDTH - 3; ++pos) {
                uint8_t bits = (i >> pos) & 7;   /* three consecutive bits */
                if (bits == 0 || bits == 2) {    /* 000 or 010 forbids */
                    ok = false;
                    break;
                }
            }
            if (ok) {
                valRows[M] = (uint8_t)i;
                raw_to_idx[i] = (int8_t)M;
                ++M;
            }
        }
        S = M * M;                             /* state space size */

        /* Allocate transition table */
        trans_counts    = (uint8_t *)malloc(S * sizeof(uint8_t));
        transitions     = (uint16_t *)malloc(S * M * sizeof(uint16_t));
        dp              = (seq_t *)calloc(S, sizeof(seq_t));
        ndp             = (seq_t *)calloc(S, sizeof(seq_t));

        /* Build transitions: (i,j) -> (j,k) if vertical rule holds */
        for (idx_t i_row = 0; i_row < M; ++i_row) {
            uint8_t row_i = valRows[i_row];
            for (idx_t j_row = 0; j_row < M; ++j_row) {
                uint8_t row_j = valRows[j_row];
                uint8_t mask_eq = (~(row_i ^ row_j)) & MASK;   /* bits where previous two equal */
                idx_t state = i_row * M + j_row;
                uint8_t count = 0;
                idx_t base = state * M;
                for (idx_t k_row = 0; k_row < M; ++k_row) {
                    uint8_t row_k = valRows[k_row];
                    if ((row_k & mask_eq) == 0) {              /* vertical rule satisfied */
                        transitions[base + count] = (uint16_t)(j_row * M + k_row);
                        ++count;
                    }
                }
                trans_counts[state] = count;
            }
        }

        /* Base answers for n = 1,2 and initialise DP for n = 2 */
        answers_size = 3;                                   /* indices 0,1,2 used */
        answers = (seq_t *)calloc(answers_size, sizeof(seq_t));
        answers[1] = (seq_t)M;
        answers[2] = (seq_t)M * (seq_t)M;
        max_precomputed_n = 2;
        curN = 2;

        for (idx_t s = 0; s < S; ++s) dp[s] = (seq_t)1;     /* after two rows any pair occurs once */

        tables_init = true;
    }

    /* Ensure answers array large enough for requested n */
    if (n + 1 > answers_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_arr = (seq_t *)realloc(answers, new_size * sizeof(seq_t));
        if (!new_arr) exit(1);
        /* initialise new entries to zero */
        memset(new_arr + answers_size, 0, (new_size - answers_size) * sizeof(seq_t));
        answers = new_arr;
        answers_size = new_size;
    }

    /* Return cached value if already computed */
    if (n <= max_precomputed_n) return answers[n];

    /* Compute DP from curN+1 up to n */
    for (idx_t step = curN + 1; step <= n; ++step) {
        /* clear ndp */
        memset(ndp, 0, S * sizeof(seq_t));

        /* iterate over all states */
        for (idx_t src = 0; src < S; ++src) {
            seq_t val = dp[src];
            if (!val) continue;
            uint8_t cnt = trans_counts[src];
            idx_t base = src * M;
            uint16_t *dest_arr = transitions + base;
            for (uint8_t t = 0; t < cnt; ++t) {
                ndp[(size_t)dest_arr[t]] += val;
            }
        }

        /* sum all counts for this step */
        seq_t sum = 0;
        for (idx_t s = 0; s < S; ++s) sum += ndp[s];
        answers[step] = sum;

        /* swap dp and ndp for next iteration */
        seq_t *tmp = dp; dp = ndp; ndp = tmp;
    }

    curN = n;
    max_precomputed_n = n;
    return answers[n];
}
