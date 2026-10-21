/*
 * A207862  Number of 7Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is computed by a transfer‑matrix dynamic‑programming
 * algorithm.  A 7×n 0/1 array is regarded as a column‑by‑column
 * construction.  Columns are 7‑bit vectors that avoid the vertical
 * patterns 010 and 101.  Two successive columns must also avoid the
 * horizontal patterns 000 and 010.  The number of such arrays is
 * obtained by building a graph on all admissible columns and
 * counting length‑n paths in this graph.
 *
 * The code uses only integer arithmetic on `seq_t` (see build
 * configuration).  It works for any `seq_t` that supports +,-,*,/ and
 * the usual comparisons and bitwise operators.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*---  Static data used for all calls -------------------------------------*/
static size_t allowed_cnt   = 0;           /* number of admissible columns    */
static uint8_t col_bits[128];              /* allowed 7‑bit vectors           */
static size_t states_cnt    = 0;           /* allowed_cnt²                    */
static size_t *offsets      = NULL;        /* start index in edges per state  */
static size_t *cnt_edges    = NULL;        /* number of outgoing edges per state */
static uint16_t *edges      = NULL;        /* concatenated list of next states  */
static seq_t *dp_cur        = NULL;        /* DP table for current length      */
static seq_t *dp_next       = NULL;        /* DP table for next length         */
static bool   initialized   = false;        /* have we built tables? */

/*--- Build phase --------------------------------------------------------*/
static void init_tables(void)
{
    /* 1. enumerate all vertical‑valid 7‑bit columns (no 010 or 101) */
    for (size_t i = 0; i < 128; ++i) {
        uint8_t v = (uint8_t)i;
        bool ok = true;
        for (size_t j = 1; j <= 5; ++j) {        /* rows j-1,j,j+1 */
            int a = (v >> (j-1)) & 1;
            int b = (v >>  j)   & 1;
            int c = (v >> (j+1)) & 1;
            if (a == c && b != a) { ok = false; break; }
        }
        if (ok) col_bits[allowed_cnt++] = v;
    }

    /* 2. prepare state space (prev2, prev1) */
    states_cnt = allowed_cnt * allowed_cnt;

    /* 3. allocate transition tables */
    offsets   = (size_t*)calloc(states_cnt, sizeof(size_t));
    cnt_edges = (size_t*)calloc(states_cnt, sizeof(size_t));

    /* 4. first pass: count edges per state */
    size_t total_edges = 0;
    for (size_t p2 = 0; p2 < allowed_cnt; ++p2) {
        uint8_t b2 = col_bits[p2];
        for (size_t p1 = 0; p1 < allowed_cnt; ++p1) {
            uint8_t b1 = col_bits[p1];
            size_t sid = p2 * allowed_cnt + p1;
            size_t cnt = 0;
            for (size_t k = 0; k < allowed_cnt; ++k) {
                uint8_t c = col_bits[k];
                bool ok = true;
                for (size_t pos = 0; pos < 7; ++pos) {
                    int bb2 = (b2 >> pos) & 1;
                    int bb1 = (b1 >> pos) & 1;
                    int bbc = (c  >> pos) & 1;
                    if ((bb2==0 && bb1==0 && bbc==0) ||
                        (bb2==0 && bb1==1 && bbc==0)) { ok = false; break; }
                }
                if (ok) ++cnt;
            }
            cnt_edges[sid] = cnt;
            total_edges   += cnt;
        }
    }

    /* 5. second pass: store actual transitions */
    edges = (uint16_t*)malloc(total_edges * sizeof(uint16_t));
    size_t cur = 0;
    for (size_t p2 = 0; p2 < allowed_cnt; ++p2) {
        uint8_t b2 = col_bits[p2];
        for (size_t p1 = 0; p1 < allowed_cnt; ++p1) {
            uint8_t b1 = col_bits[p1];
            size_t sid = p2 * allowed_cnt + p1;
            offsets[sid] = cur;
            for (size_t k = 0; k < allowed_cnt; ++k) {
                uint8_t c = col_bits[k];
                bool ok = true;
                for (size_t pos = 0; pos < 7; ++pos) {
                    int bb2 = (b2 >> pos) & 1;
                    int bb1 = (b1 >> pos) & 1;
                    int bbc = (c  >> pos) & 1;
                    if ((bb2==0 && bb1==0 && bbc==0) ||
                        (bb2==0 && bb1==1 && bbc==0)) { ok = false; break; }
                }
                if (ok) edges[cur++] = (uint16_t)k;
            }
        }
    }

    /* 6. allocate DP tables (no initialisation needed here) */
    dp_cur  = (seq_t*)malloc(states_cnt * sizeof(seq_t));
    dp_next = (seq_t*)malloc(states_cnt * sizeof(seq_t));

    initialized = true;
}

/*---  Main function -----------------------------------------------------*/
seq_t a(idx_t n)
{
    if (!initialized) init_tables();

    if (n == 1) return (seq_t)allowed_cnt;
    if (n == 2) return (seq_t)allowed_cnt * (seq_t)allowed_cnt;

    /* initialise dp_cur for length 2: every pair of columns is allowed */
    for (size_t i = 0; i < states_cnt; ++i) dp_cur[i] = 1;

    /* iterate from length 3 up to n */
    for (idx_t cur_n = 3; cur_n <= n; ++cur_n) {
        /* zero dp_next */
        for (size_t i = 0; i < states_cnt; ++i) dp_next[i] = 0;

        /* propagate transitions */
        for (size_t p2 = 0; p2 < allowed_cnt; ++p2) {
            for (size_t p1 = 0; p1 < allowed_cnt; ++p1) {
                size_t sid = p2 * allowed_cnt + p1;
                seq_t cnt = dp_cur[sid];
                if (!cnt) continue;
                size_t base  = p1 * allowed_cnt;     /* new prev2 = p1     */
                size_t off   = offsets[sid];
                size_t total = cnt_edges[sid];
                for (size_t e = 0; e < total; ++e) {
                    uint16_t k = edges[off + e];
                    dp_next[base + k] += cnt;
                }
            }
        }

        /* swap dp_cur and dp_next for next iteration */
        seq_t *tmp = dp_cur; dp_cur = dp_next; dp_next = tmp;
    }

    /* sum all states -> total number of arrays of width n */
    seq_t total = 0;
    for (size_t i = 0; i < states_cnt; ++i) total += dp_cur[i];
    return total;
}
