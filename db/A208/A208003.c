/*
 * A208003  Number of n X 4 0..1 arrays avoiding 0 0 1 and 1 0 0 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A208003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
   A208003 – Number of n × 4 0..1 arrays avoiding 0 0 1 and 1 0 0 horizontally and
   0 1 1 and 1 1 0 vertically.

   A DP formulation:  each row must itself avoid the horizontal patterns
   (there are 9 such rows).  The vertical restriction only couples
   three consecutive rows: if the middle row has a 1 in a column then the
   bits in that column of the two adjacent rows must be equal.
   The DP keeps the last two rows seen; there are 9 × 9 = 81 possible
   states.  Transition from state (p2,p1) to (p1,r) is allowed iff r
   satisfies the vertical rule with p2 and p1.
   For n=1 the answer is 9; for n=2 it is 9×9=81.
   For n≥3 the DP recurses over the required number of rows.
*/

#include <stddef.h>

static const seq_t VALID_ROW[9] = {
    0, 5, 6, 7, 10, 11, 13, 14, 15   /* 0000,0101,0110,0111,1010,1011,1101,1110,1111 */
};

/* Transition table: allowed[p2][p1][r] == 1 if r is a valid third row
   given previous rows p2 (row i-2) and p1 (row i-1).
   9×9×9 booleans = 729 bytes */
static unsigned char TRANSITION[9][9][9];
static unsigned char TRANSITION_INITIALIZED = 0;

static void init_transition(void) {
    if (TRANSITION_INITIALIZED) return;
    for (int p2 = 0; p2 < 9; ++p2) {
        for (int p1 = 0; p1 < 9; ++p1) {
            for (int r = 0; r < 9; ++r) {
                int ok = 1;
                seq_t v2 = VALID_ROW[p2];
                seq_t v1 = VALID_ROW[p1];
                seq_t vr = VALID_ROW[r];
                for (int pos = 0; pos < 4; ++pos) {
                    if (((v1 >> pos) & 1) == 1) {
                        /* Middle bit is 1 – bits of v2 and vr must coincide */
                        if (((v2 >> pos) & 1) != ((vr >> pos) & 1)) {
                            ok = 0;
                            break;
                        }
                    }
                }
                TRANSITION[p2][p1][r] = ok;
            }
        }
    }
    TRANSITION_INITIALIZED = 1;
}

/* dp_old and dp_new index: state = p2*9 + p1   (p2 previous-2 row, p1 previous-1 row) */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* out of range; not used by the tests */
    if (n == 1) return 9;
    if (n == 2) return 81;         /* all pairs of valid rows */

    init_transition();

    const size_t STATES = 81;
    seq_t dp_old[STATES];
    seq_t dp_new[STATES];

    /* initialize dp for two rows: every pair is allowed once */
    for (size_t s = 0; s < STATES; ++s) dp_old[s] = 1;

    for (idx_t row = 3; row <= n; ++row) {
        for (size_t s = 0; s < STATES; ++s) dp_new[s] = 0;

        for (int p2 = 0; p2 < 9; ++p2) {
            for (int p1 = 0; p1 < 9; ++p1) {
                size_t old_state = p2 * 9 + p1;
                seq_t cur_cnt = dp_old[old_state];
                if (!cur_cnt) continue;
                for (int r = 0; r < 9; ++r) {
                    if (!TRANSITION[p2][p1][r]) continue;
                    size_t new_state = p1 * 9 + r;
                    dp_new[new_state] += cur_cnt;
                }
            }
        }
        for (size_t s = 0; s < STATES; ++s) dp_old[s] = dp_new[s];
    }

    /* sum all final states */
    seq_t sum = 0;
    for (size_t s = 0; s < STATES; ++s) sum += dp_old[s];
    return sum;
}
