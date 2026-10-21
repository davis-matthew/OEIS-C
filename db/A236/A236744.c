/*
 * A236744  Number of (n+1)X(6+1) 0..2 arrays with the upper median plus the
 *          lower median of every 2X2 subblock equal.
 * https://oeis.org/A236744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{S=0}^{4} count_S(n) where count_S(n) counts the
 *            (n+1)×7 arrays over {0,1,2} such that for every 2×2 subblock the
 *            second and third smallest values (lower + upper median) sum to
 *            the same constant S. A dynamic‑programming algorithm over rows
 *            is used
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define ROWS 2187        /* 3^7 possible rows */
#define SBITS 5          /* S ranges 0..4 */
#define TOTAL_STATES (ROWS*SBITS)

/* row representation in base‑3 */
static unsigned char row_val[ROWS][7];

/* adjacency lists for each (S,row) pair */
static unsigned int *state_start;     /* size TOTAL_STATES */
static unsigned short *state_count;   /* size TOTAL_STATES */
static unsigned short *nb_list;       /* concatenated neighbor rows */
static size_t nb_len, nb_cap;

/* table: sum of the middle two sorted values of a 2×2 block */
static unsigned short medsum_table[81];

/* one‑time initialization flag */
static bool adjacency_built = false;

/* compute medsum_table */
static void init_medsum_table(void)
{
    for (int i = 0; i < 81; ++i) {
        int v0 = i / 27;
        int v1 = (i / 9) % 3;
        int v2 = (i / 3) % 3;
        int v3 = i % 3;
        int a[4] = {v0, v1, v2, v3};
        /* simple sorting */
        for (int p = 0; p < 3; ++p)
            for (int q = p + 1; q < 4; ++q)
                if (a[p] > a[q]) {
                    int t = a[p]; a[p] = a[q]; a[q] = t;
                }
        medsum_table[i] = (unsigned short)(a[1] + a[2]); /* lower+upper median */
    }
}

/* build adjacency lists for all S and rows once */
static void build_adjacency(void)
{
    if (adjacency_built) return;

    /* compute row values */
    for (int r = 0; r < ROWS; ++r) {
        int tmp = r;
        for (int p = 0; p < 7; ++p) {
            row_val[r][p] = (unsigned char)(tmp % 3);
            tmp /= 3;
        }
    }

    /* allocate arrays */
    state_start = (unsigned int *)malloc(TOTAL_STATES * sizeof(unsigned int));
    state_count = (unsigned short *)malloc(TOTAL_STATES * sizeof(unsigned short));
    nb_cap = 4000000;       /* initial guess */
    nb_list = (unsigned short *)malloc(nb_cap * sizeof(unsigned short));
    nb_len = 0;

    init_medsum_table();

    /* build lists */
    for (int S = 0; S < SBITS; ++S) {
        for (int a = 0; a < ROWS; ++a) {
            unsigned int start = nb_len;
            unsigned short cnt = 0;
            for (int b = 0; b < ROWS; ++b) {
                bool ok = true;
                for (int j = 0; j < 6; ++j) {
                    int idx = row_val[a][j] * 27 +
                              row_val[a][j + 1] * 9 +
                              row_val[b][j] * 3 +
                              row_val[b][j + 1];
                    if (medsum_table[idx] != (unsigned short)S) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    if (nb_len == nb_cap) {
                        nb_cap <<= 1;
                        nb_list = (unsigned short *)realloc(nb_list, nb_cap * sizeof(unsigned short));
                    }
                    nb_list[nb_len++] = (unsigned short)b;
                    ++cnt;
                }
            }
            state_start[S * ROWS + a] = start;
            state_count[S * ROWS + a] = cnt;
        }
    }
    adjacency_built = true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    build_adjacency();

    /* DP arrays reused for each S */
    static seq_t *dp_cur = NULL, *dp_next = NULL;
    static size_t dp_cap = 0;
    if (dp_cap < ROWS) {
        dp_cur = (seq_t *)calloc(ROWS, sizeof(seq_t));
        dp_next = (seq_t *)calloc(ROWS, sizeof(seq_t));
        dp_cap = ROWS;
    }
    seq_t result = 0;

    for (int S = 0; S < SBITS; ++S) {
        /* initialise dp_cur: any first row */
        for (int i = 0; i < ROWS; ++i) dp_cur[i] = (seq_t)1;

        for (idx_t step = 1; step <= n; ++step) {
            memset(dp_next, 0, ROWS * sizeof(seq_t));
            for (int a = 0; a < ROWS; ++a) {
                seq_t ways = dp_cur[a];
                if (!ways) continue;
                unsigned int idx0 = S * ROWS + a;
                unsigned int start = state_start[idx0];
                unsigned short cnt = state_count[idx0];
                unsigned short *nbr = nb_list + start;
                for (unsigned short k = 0; k < cnt; ++k) {
                    unsigned short b = nbr[k];
                    dp_next[b] += ways;
                }
            }
            /* swap buffers */
            seq_t *tmp = dp_cur;
            dp_cur = dp_next;
            dp_next = tmp;
        }

        /* sum all paths of length n for this S */
        seq_t sub = 0;
        for (int i = 0; i < ROWS; ++i) sub += dp_cur[i];
        result += sub;
    }
    return result;
}
