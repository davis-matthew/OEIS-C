/*
 * A238293  Number of (n+1) X (1+1) 0..3 arrays with the maximum plus the
 *          upper median minus the lower median of every 2 X 2 subblock
 *          differing from its horizontal and vertical neighbors by exactly
 *          one.
 * https://oeis.org/A238293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A238293: Number of (n+1)×2 arrays with entries 0..3 such that the value
   max + upper median − lower median of every 2×2 subblock differs by exactly
   one from that of its vertically adjacent subblock.    */

/* The sequence satisfies the following recurrence:
   Let f(r₁,r₂) be the value max + upper median − lower median of the block
   formed by two consecutive rows r₁,r₂ (each row is two digits 0..3).
   For n≥2,
      a(n) = Σ₍rₙ₋₁,rₙ₎  Σ₍vₙ₋₁,vₙ₎  a_aux(rₙ₋₁,rₙ,vₙ₋₁,vₙ)
   where a_aux counts arrays of length n+1 and the condition |vₙ−vₙ₁|=1
   holds for every consecutive pair of blocks.  This DP is implemented
   directly below.                                      */

#include <stdint.h>
#include <stdbool.h>

/* Helper: value of a 2×2 block.  Result is in 0..6. */
static unsigned char block_value(int a, int b, int c, int d)
{
    unsigned arr[4] = {a, b, c, d};
    /* simple bubble sort */
    for (int i = 0; i < 4; ++i)
        for (int j = i + 1; j < 4; ++j)
            if (arr[i] > arr[j]) {
                int t = arr[i];
                arr[i] = arr[j];
                arr[j] = t;
            }
    /* sorted as arr[0] ≤ arr[1] ≤ arr[2] ≤ arr[3] */
    return arr[3] + arr[2] - arr[1];  /* max + upper median - lower median */
}

/* Pre‑computed table for all pairs of rows (16×16). */
static unsigned char val[16][16];
static bool init_val = false;

static void init_val_table(void)
{
    if (init_val) return;
    for (int r1 = 0; r1 < 16; ++r1) {
        int a1 = r1 / 4;          /* left column of first row */
        int b1 = r1 % 4;          /* right column of first row */
        for (int r2 = 0; r2 < 16; ++r2) {
            int a2 = r2 / 4;      /* left column of second row */
            int b2 = r2 % 4;      /* right column of second row */
            val[r1][r2] = block_value(a1, b1, a2, b2);
        }
    }
    init_val = true;
}

/* Core function: returns a(n) for n ≥ 1.  Complexity O(n). */
seq_t a(idx_t n)
{
    if (n == 0) {  /* offset is 1; returning 0 for n==0 is harmless */
        return (seq_t)0;
    }

    init_val_table();

    /* dp_prev[r][v] = number of arrays up to current row where
       the last row is r and the value of the last block is v. */
    seq_t dp_prev[16][7] = {0};
    seq_t dp_cur [16][7] = {0};

    /* Base case: arrays of size 2×2 (n = 1).  Build all 16×16 row pairs. */
    for (int r0 = 0; r0 < 16; ++r0) {
        for (int r1 = 0; r1 < 16; ++r1) {
            unsigned char v = val[r0][r1];
            dp_prev[r1][v] += 1;
        }
    }

    /* If only one block, sum everything and return. */
    if (n == 1) {
        seq_t sum = 0;
        for (int r = 0; r < 16; ++r)
            for (int v = 0; v < 7; ++v)
                sum += dp_prev[r][v];
        return sum;
    }

    /* For n ≥ 2, add rows one by one. */
    for (int row = 2; row <= (int)n; ++row) {
        /* Clear dp_cur */
        for (int r = 0; r < 16; ++r)
            for (int v = 0; v < 7; ++v)
                dp_cur[r][v] = 0;

        /* Transition from dp_prev to dp_cur */
        for (int r_prev = 0; r_prev < 16; ++r_prev) {
            for (int v_prev = 0; v_prev < 7; ++v_prev) {
                seq_t cnt = dp_prev[r_prev][v_prev];
                if (cnt == 0) continue;
                for (int r_curr = 0; r_curr < 16; ++r_curr) {
                    unsigned char v_cur = val[r_prev][r_curr];
                    if (v_cur == v_prev + 1 || v_cur == v_prev - 1) {
                        dp_cur[r_curr][v_cur] += cnt;
                    }
                }
            }
        }

        /* Prepare for next iteration */
        for (int r = 0; r < 16; ++r)
            for (int v = 0; v < 7; ++v)
                dp_prev[r][v] = dp_cur[r][v];
    }

    /* Sum all counts for the final row. */
    seq_t total = 0;
    for (int r = 0; r < 16; ++r)
        for (int v = 0; v < 7; ++v)
            total += dp_prev[r][v];
    return total;
}
