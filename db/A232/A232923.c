/*
 * A232923  Number of 4 X n 0..2 arrays with no element x(i,j) adjacent to
 *          value 2-x(i,j) horizontally, diagonally or antidiagonally.
 * https://oeis.org/A232923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A232923 – Number of 4×n arrays with values 0..2 such that
    no element is adjacent to its complement (2‑x) horizontally,
    diagonally or antidiagonally.
    The sequence satisfies the empirical recurrence

        a(n) = 3*a(n‑1) + 16*a(n‑2) – 3*a(n‑3) – 25*a(n‑4) + 2*a(n‑5) + 4*a(n‑6)
        for n > 7,
    with initial terms

        a(1) = 81,  a(2) = 162,  a(3) = 1122,  a(4) = 5808,
        a(5) = 33702, a(6) = 186720, a(7) = 1054446.

    The implementation below does not use the recurrence directly;
    it computes the terms by dynamic programming over the 3⁴ (=81)
    possible column configurations.  Each state is a 4‑digit base‑3
    number representing the values in a column.  From a state S the
    allowed successor states are those where each column value does
    not complement any of the three adjacent values in the previous
    column.  The DP runs in O(81·n) time and O(81) memory. */

#include <stdint.h>

/* -------------------------------------------------------------------- */
/*  Precomputation of state tables (once per program execution) */

/*  State 0..80: 4 digits base‑3 (row0 is least significant). */
static int val[81][4];          /* val[s][r] = value in row r of state s */
static int succ[81][81];        /* succ[s][k] = k‑th successor state of s    */
static int succ_cnt[81];        /* succ_cnt[s] = number of successors of s  */
static int pow3[4] = {1, 3, 9, 27};
static int tables_ready = 0;

static void build_tables(void)
{
    if (tables_ready) return;

    /* Map state number to row values */
    for (int s = 0; s < 81; ++s) {
        int tmp = s;
        for (int r = 0; r < 4; ++r) {
            val[s][r] = tmp % 3;
            tmp /= 3;
        }
    }

    /* Compute successors for each state */
    for (int s = 0; s < 81; ++s) {
        /* allowed[r] is a 3‑bit mask of values allowed in row r of the
           next column given state s. */
        int allowed[4];
        for (int r = 0; r < 4; ++r) {
            int forb = 0;
            if (r > 0)     forb |= 1 << (2 - val[s][r - 1]);   /* upper diagonal */
            forb |= 1 << (2 - val[s][r]);                    /* horizontal */
            if (r < 3)     forb |= 1 << (2 - val[s][r + 1]);   /* lower diagonal */
            allowed[r] = (~forb) & 0x7;                     /* keep 3 bits */
        }

        succ_cnt[s] = 0;
        /* Enumerate all combinations of allowed values for the next column */
        for (int v0 = 0; v0 < 3; ++v0)
            if (allowed[0] & (1 << v0))
                for (int v1 = 0; v1 < 3; ++v1)
                    if (allowed[1] & (1 << v1))
                        for (int v2 = 0; v2 < 3; ++v2)
                            if (allowed[2] & (1 << v2))
                                for (int v3 = 0; v3 < 3; ++v3)
                                    if (allowed[3] & (1 << v3)) {
                                        int nxt = v0 + v1 * 3 + v2 * 9 + v3 * 27;
                                        succ[s][succ_cnt[s]++] = nxt;
                                    }
    }

    tables_ready = 1;
}

/* -------------------------------------------------------------------- */
/*  Main sequence routine  */

seq_t a(idx_t n)
{
    /* Ensure tables are built once */
    build_tables();

    /* DP arrays – n ≥ 1 */
    seq_t cur[81];
    seq_t nxt[81];

    /* Initial column: all 81 states are possible */
    for (int s = 0; s < 81; ++s) cur[s] = (seq_t)1;

    /* Process remaining columns */
    for (idx_t col = 2; col <= n; ++col) {
        for (int s = 0; s < 81; ++s) nxt[s] = (seq_t)0;

        for (int s = 0; s < 81; ++s) {
            seq_t ways = cur[s];
            if (ways == (seq_t)0) continue;

            for (int k = 0; k < succ_cnt[s]; ++k) {
                int t = succ[s][k];
                nxt[t] += ways;
            }
        }

        /* Copy next to current for next iteration */
        for (int s = 0; s < 81; ++s) cur[s] = nxt[s];
    }

    /* Sum over all final states */
    seq_t result = (seq_t)0;
    for (int s = 0; s < 81; ++s) result += cur[s];
    return result;
}
