/*
 * A207418  Number of nX7 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A207418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 27, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: Count n×7 binary arrays avoiding the forbidden
   horizontal patterns 001 and 010 and the vertical patterns 011
   and 101.  Uses a transfer‑matrix dynamic programming over rows. */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. */
    if (n < 1) return 0;

    /* For n = 1 the number equals the number of allowable rows. */
    if (n == 1) return (seq_t)31;

    /* Enumerate all 7-bit rows that avoid 001 and 010. */
    uint8_t allowed[31];
    int allowed_cnt = 0;
    for (uint16_t m = 0; m < 128; ++m) {
        bool ok = true;
        for (int j = 0; j <= 4; ++j) {          /* 3-bit windows 0..4 */
            uint8_t sub = (m >> j) & 7;         /* 3 bits */
            if (sub == 1 || sub == 2) {         /* 001 or 010 */
                ok = false;
                break;
            }
        }
        if (ok) allowed[allowed_cnt++] = (uint8_t)m;
    }

    /* Map a row value to its index in the allowed array. */
    uint8_t row_index[128];
    for (int i = 0; i < 128; ++i) row_index[i] = 255;
    for (int i = 0; i < allowed_cnt; ++i) row_index[allowed[i]] = (uint8_t)i;

    /* Pre‑compute pair masks and list of compatible next rows. */
    uint8_t pair_mask[31][31];
    uint8_t next_list[31*31][31];     /* maximum 31 entries per pair */
    uint8_t next_len[31*31];
    for (int i = 0; i < allowed_cnt; ++i) {
        for (int j = 0; j < allowed_cnt; ++j) {
            uint8_t mask = allowed[i] ^ allowed[j];
            pair_mask[i][j] = mask;
            uint8_t len = 0;
            for (int k = 0; k < allowed_cnt; ++k) {
                if ((allowed[k] & mask) == 0) {
                    next_list[i*31 + j][len++] = (uint8_t)k;
                }
            }
            next_len[i*31 + j] = len;
        }
    }

    /* Dynamic programming over rows. */
    const int STATE_SIZE = 31 * 31;
    seq_t dp_cur[STATE_SIZE];
    seq_t dp_next[STATE_SIZE];

    /* For n = 2, every pair of allowed rows is legal. */
    for (int i = 0; i < STATE_SIZE; ++i) dp_cur[i] = (seq_t)1;

    /* Iterate from the third row up to row n. */
    for (idx_t row = 3; row <= n; ++row) {
        /* Zero next state array. */
        for (int i = 0; i < STATE_SIZE; ++i) dp_next[i] = (seq_t)0;

        for (int i = 0; i < STATE_SIZE; ++i) {
            seq_t cnt = dp_cur[i];
            if (cnt == 0) continue;
            int p1 = i / 31;
            int p2 = i % 31;
            uint8_t *list = next_list[i];
            uint8_t len   = next_len[i];
            for (int t = 0; t < len; ++t) {
                int r3 = list[t];
                int nxt = p2 * 31 + r3;
                dp_next[nxt] += cnt;
            }
        }
        /* Move to the next row. */
        for (int i = 0; i < STATE_SIZE; ++i) dp_cur[i] = dp_next[i];
    }

    /* Sum all final states to get the total number of arrays. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < STATE_SIZE; ++i) total += dp_cur[i];
    return total;
}
