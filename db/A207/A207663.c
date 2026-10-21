/*
 * A207663  Number of 5Xn 0..1 arrays avoiding 0 0 0 and 1 1 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: Counts 5×n binary arrays avoiding horizontal 000/111
 * and vertical 001/100 by dynamic programming over columns, using the
 * state of the previous two columns. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Pre‑compute vertical‑valid columns (no 001 or 100 inside) and the set
 * of transitions that satisfy the horizontal 000/111 rule for triples
 * of consecutive columns.  This is done only once. */
static bool valid[32];
static int valids_list[32];
static size_t valids_count;
static bool trig[32][32][32];
static bool initialized = false;

seq_t a(idx_t n)
{
    /* --- 1. Pre‑computation of states and transitions  ----------- */
    if (!initialized) {
        /* Find all vertical‑valid 5‑bit columns. */
        valids_count = 0;
        for (int s = 0; s < 32; ++s) {
            bool ok = true;
            for (int r = 0; r <= 2; ++r) {
                int b0 = (s >> r) & 1;
                int b1 = (s >> (r + 1)) & 1;
                int b2 = (s >> (r + 2)) & 1;
                if ((b0 == 0 && b1 == 0 && b2 == 1) ||
                    (b0 == 1 && b1 == 0 && b2 == 0)) {
                    ok = false;
                    break;
                }
            }
            valid[s] = ok;
            if (ok) {
                valids_list[valids_count++] = s;
            }
        }

        /* Triples (a,b,c) are admissible iff every row in a,b,c
         * does not equal 000 or 111. */
        for (int a_i = 0; a_i < 32; ++a_i) {
            for (int b_i = 0; b_i < 32; ++b_i) {
                for (int c_i = 0; c_i < 32; ++c_i) {
                    if (!valid[a_i] || !valid[b_i] || !valid[c_i]) {
                        trig[a_i][b_i][c_i] = false;
                        continue;
                    }
                    bool ok = true;
                    for (int r = 0; r < 5; ++r) {
                        int ba = (a_i >> r) & 1;
                        int bb = (b_i >> r) & 1;
                        int bc = (c_i >> r) & 1;
                        if (ba == bb && bb == bc) {
                            ok = false;
                            break;
                        }
                    }
                    trig[a_i][b_i][c_i] = ok;
                }
            }
        }
        initialized = true;
    }

    /* --- 2. Handle the trivial small cases ----------------------- */
    if (n == 1) {
        /* The number of vertical‑valid 5‑bit columns. */
        return (seq_t)valids_count;
    }
    if (n == 2) {
        /* Any pair of vertical‑valid columns is allowed. */
        return (seq_t)valids_count * (seq_t)valids_count;
    }

    /* --- 3. Dynamic programming over columns --------------------- */
    /* dp_current[x] and dp_next[y] store counts for column pairs
     * (a,b) where x = a*32 + b, y = b*32 + c.  sizes are 32*32 = 1024. */
    static seq_t dp_current[32 * 32];
    static seq_t dp_next[32 * 32];
    const size_t ST = 32 * 32;

    /* Initialize for n = 2: each vertical‑valid pair contributes 1. */
    for (size_t i = 0; i < ST; ++i)
        dp_current[i] = (seq_t)0;
    for (int a_i = 0; a_i < 32; ++a_i) if (valid[a_i]) {
        for (int b_i = 0; b_i < 32; ++b_i) if (valid[b_i]) {
            dp_current[(size_t)a_i * 32 + b_i] = (seq_t)1;
        }
    }

    /* Process subsequent columns (3 .. n). */
    for (idx_t step = 3; step <= n; ++step) {
        for (size_t i = 0; i < ST; ++i)
            dp_next[i] = (seq_t)0;
        for (int a_i = 0; a_i < 32; ++a_i) if (valid[a_i]) {
            for (int b_i = 0; b_i < 32; ++b_i) if (valid[b_i]) {
                size_t idx = (size_t)a_i * 32 + b_i;
                seq_t cur = dp_current[idx];
                if (cur == (seq_t)0) continue;
                for (size_t vi = 0; vi < valids_count; ++vi) {
                    int c_i = valids_list[vi];
                    if (!trig[a_i][b_i][c_i]) continue;
                    size_t next_idx = (size_t)b_i * 32 + c_i;
                    dp_next[next_idx] += cur;
                }
            }
        }
        /* Move dp_next into dp_current for the next iteration. */
        for (size_t i = 0; i < ST; ++i)
            dp_current[i] = dp_next[i];
    }

    /* --- 4. Sum over all possible final column pairs. ---------- */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < ST; ++i)
        result += dp_current[i];
    return result;
}
