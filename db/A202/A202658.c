/*
 * A202658  Number of -2..2 arrays of n elements with first through fourth
 *          differences also in -2..2.
 * https://oeis.org/A202658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 35, u64 n <= 75, u128 n <= 154,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by dynamic programming over the last four values of the array.
   The DP state is the tuple of the last four elements (in the range -2..2).
   For lengths n ≤ 4 separate tables of size 5, 25, 125, 625 are used;
   for n ≥ 5 only the 4‑tuple table of size 625 is needed.  This provides
   a linear‑time algorithm in n.  */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static state to allow arbitrary order of calls */
    static idx_t current_len = 0;       /* maximum length already computed   */
    static seq_t *a_arr = NULL;         /* array of a values, index from 1   */
    static size_t a_cap   = 0;          /* capacity of a_arr                 */

    static seq_t counts1[5];            /* states for length 1 */
    static seq_t counts2[25];           /* states for length 2 */
    static seq_t counts3[125];          /* states for length 3 */
    static seq_t *counts4 = NULL;       /* states for length ≥4, 4‑tuple */
    static seq_t *new_counts4 = NULL;   /* temporary table for the transition */

    /* --- allocate a_arr ------------------- */
    if (!a_arr) {
        a_cap = 64;
        a_arr = (seq_t*)calloc(a_cap, sizeof(seq_t));
        if (!a_arr) abort();
    }

    /* --- allocate counts4 ----------------- */
    if (!counts4) {
        counts4 = (seq_t*)calloc(625, sizeof(seq_t));
        new_counts4 = (seq_t*)calloc(625, sizeof(seq_t));
        if (!counts4 || !new_counts4) abort();
    }

    /* --- grow a_arr if necessary --------- */
    while (a_cap <= n) {
        size_t new_cap = a_cap * 2;
        seq_t *tmp = (seq_t*)realloc(a_arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        /* zero the new part */
        for (size_t i = a_cap; i < new_cap; ++i) tmp[i] = 0;
        a_arr = tmp;
        a_cap = new_cap;
    }

    /* --- compute up to requested n  -------- */
    while (current_len < n) {
        idx_t next = current_len + 1;  /* length to compute */

        if (current_len == 0) {        /* length 1 */
            for (int d = 0; d < 5; ++d) counts1[d] = 1;
            a_arr[1] = 5;
            current_len = 1;

        } else if (current_len == 1) { /* length 2 */
            for (int i = 0; i < 25; ++i) counts2[i] = 0;
            for (int d1 = 0; d1 < 5; ++d1) {
                seq_t c1 = counts1[d1];
                if (!c1) continue;
                for (int v = 0; v < 5; ++v) {
                    if (abs(v - d1) <= 2)
                        counts2[d1 * 5 + v] += c1;
                }
            }
            seq_t sum = 0;
            for (int i = 0; i < 25; ++i) sum += counts2[i];
            a_arr[2] = sum;
            current_len = 2;

        } else if (current_len == 2) { /* length 3 */
            for (int i = 0; i < 125; ++i) counts3[i] = 0;
            for (int idx = 0; idx < 25; ++idx) {
                seq_t c = counts2[idx];
                if (!c) continue;
                int d1 = idx / 5;
                int d2 = idx % 5;
                for (int v = 0; v < 5; ++v) {
                    if (abs(v - d2) <= 2) {
                        int diff2 = v - 2 * d2 + d1;
                        if (abs(diff2) <= 2) {
                            int idx3 = d1 * 25 + d2 * 5 + v;
                            counts3[idx3] += c;
                        }
                    }
                }
            }
            seq_t sum = 0;
            for (int i = 0; i < 125; ++i) sum += counts3[i];
            a_arr[3] = sum;
            current_len = 3;

        } else if (current_len == 3) { /* length 4 */
            for (int i = 0; i < 625; ++i) counts4[i] = 0;
            for (int idx = 0; idx < 125; ++idx) {
                seq_t c = counts3[idx];
                if (!c) continue;
                int d1 = idx / 25;
                int d2 = (idx / 5) % 5;
                int d3 = idx % 5;
                for (int v = 0; v < 5; ++v) {
                    if (abs(v - d3) <= 2) {
                        int diff2 = v - 2 * d3 + d2;
                        if (abs(diff2) <= 2) {
                            int diff3 = v - 3 * d3 + 3 * d2 - d1;
                            if (abs(diff3) <= 2) {
                                int idx4 = d1 * 125 + d2 * 25 + d3 * 5 + v;
                                counts4[idx4] += c;
                            }
                        }
                    }
                }
            }
            seq_t sum = 0;
            for (int i = 0; i < 625; ++i) sum += counts4[i];
            a_arr[4] = sum;
            current_len = 4;

        } else {                       /* length ≥5 */
            /* tmp array is already zeroed */
            for (int i = 0; i < 625; ++i) new_counts4[i] = 0;

            for (int idx = 0; idx < 625; ++idx) {
                seq_t c = counts4[idx];
                if (!c) continue;
                int d1 = idx / 125;
                int d2 = (idx / 25) % 5;
                int d3 = (idx / 5) % 5;
                int d4 = idx % 5;
                for (int v = 0; v < 5; ++v) {
                    if (abs(v - d4) <= 2) {
                        int diff2 = v - 2 * d4 + d3;
                        if (abs(diff2) <= 2) {
                            int diff3 = v - 3 * d4 + 3 * d3 - d2;
                            if (abs(diff3) <= 2) {
                                int diff4 = v - 4 * d4 + 6 * d3 - 4 * d2 + d1;
                                if (abs(diff4) <= 2) {
                                    int new_idx = ((idx % 125) * 5) + v;
                                    new_counts4[new_idx] += c;
                                }
                            }
                        }
                    }
                }
            }

            /* move new_counts4 back to counts4 */
            for (int i = 0; i < 625; ++i) counts4[i] = new_counts4[i];

            seq_t sum = 0;
            for (int i = 0; i < 625; ++i) sum += counts4[i];
            a_arr[next] = sum;
            current_len = next;
        }
    }

    return a_arr[n];
}
