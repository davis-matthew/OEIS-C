/*
 * A164446  Number of binary strings of length n with no substrings equal to
 *          0001 0010 or 0101.
 * https://oeis.org/A164446
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of binary strings of length n with no substrings
 *            0001, 0010, 0101
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 42, u64 n <= 86, u128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* The automaton has 15 states: 1 + 2 + 4 + 8 = 15.
   state id = offset[len] + bits, where len = 0..3 is the length
   of the stored suffix and bits = 0..2^len-1 are the suffix bits.
*/
static const int offset[4] = {0, 1, 3, 7};

static bool init_done = false;
static int trans_tbl[15][2];

/* Build the transition table once. */
static void init_automaton(void)
{
    if (init_done) return;

    const unsigned forbidden[3] = {1, 2, 5};   /* 0001, 0010, 0101 */

    for (int len = 0; len <= 3; ++len) {
        int maxbits = 1 << len;
        for (int bits = 0; bits < maxbits; ++bits) {
            int state = offset[len] + bits;
            for (int bit = 0; bit < 2; ++bit) {
                if (len == 3) {
                    int new_bits = ((bits << 1) | bit) & 7;
                    int full4   = ((bits << 1) | bit) & 15;
                    bool is_forbidden = false;
                    for (int f = 0; f < 3; ++f)
                        if (full4 == forbidden[f]) { is_forbidden = true; break; }
                    if (is_forbidden)
                        trans_tbl[state][bit] = -1;
                    else
                        trans_tbl[state][bit] = offset[3] + new_bits;
                } else {
                    int new_len   = len + 1;
                    int new_bits  = ((bits << 1) | bit) & ((1 << new_len) - 1);
                    trans_tbl[state][bit] = offset[new_len] + new_bits;
                }
            }
        }
    }
    init_done = true;
}

/* Compute a(n) for any n >= 0.  The result is correct for the OEIS
   sequence starting at offset 4; for n < 4 all strings are allowed,
   so the counts are 2^n. */
seq_t a(idx_t n)
{
    init_automaton();

    static seq_t prev[15];
    static seq_t curr[15];

    /* initialize for length 0 */
    for (int s = 0; s < 15; ++s) prev[s] = 0;
    prev[0] = 1;   /* only the empty string exists */

    for (idx_t pos = 1; pos <= n; ++pos) {
        /* current column */
        for (int s = 0; s < 15; ++s) curr[s] = 0;
        /* transition from previous column */
        for (int s = 0; s < 15; ++s) {
            seq_t val = prev[s];
            if (val == 0) continue;
            for (int bit = 0; bit < 2; ++bit) {
                int ns = trans_tbl[s][bit];
                if (ns == -1) continue;   /* leads to a forbidden substring */
                curr[ns] += val;
            }
        }
        /* copy back */
        for (int s = 0; s < 15; ++s) prev[s] = curr[s];
    }

    /* sum all state counts for the final length */
    seq_t total = 0;
    for (int s = 0; s < 15; ++s) total += prev[s];
    return total;
}
