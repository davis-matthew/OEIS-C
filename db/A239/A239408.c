/*
 * A239408  Number of 4Xn 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or two plus the sum of the elements
 *          above it, modulo 4.
 * https://oeis.org/A239408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  Formula: a(n) = number of 4×n arrays over {0,1,2,3} with
 *      no entry equal to (1 + sum to its left) mod 4
 *      nor to (2 + sum above it) mod 4.
 *
 *  The implementation uses a 256‑state dynamic program:
 *    * state s is a base‑4 number whose i‑th digit (0..3)
 *      is the sum modulo 4 of the first i+1 rows after
 *      processing the current column.
 *    * For each state we pre‑compute all possible next states
 *      and the number of assignments that realise the
 *      transition.
 *  Complexity: O(n·256²) (~65 k operations per column);
 *  well below the time limit for n ≤ 10000.
 */

#include <stdbool.h>
#include <stdint.h>

/* Transition counts: trans[s][t] = number of assignments
 * taking state s to state t in one column. */
static uint16_t trans[256][256];

/* Row sums modulo 4 for every state.  row_sum[s][i]
 * is the sum modulo 4 of the i‑th row after processing
 * the columns represented by state s. */
static uint8_t row_sum[256][4];

/* Flag to ensure the transition tables are computed only once. */
static bool tables_ready = false;

/* Build transition tables once. */
static void build_tables(void)
{
    if (tables_ready)
        return;

    /* Decode each state's row sums.  State numbering is base‑4: */
    for (int s = 0; s < 256; ++s)
    {
        int x = s;
        for (int i = 0; i < 4; ++i)
        {
            row_sum[s][i] = x & 3;        /* take two bits */
            x >>= 2;
        }
    }

    /* Enumerate all assignments for a column (4⁴ = 256). */
    for (int s = 0; s < 256; ++s)
    {
        for (int a0 = 0; a0 < 4; ++a0)
            for (int a1 = 0; a1 < 4; ++a1)
                for (int a2 = 0; a2 < 4; ++a2)
                    for (int a3 = 0; a3 < 4; ++a3)
                    {
                        uint8_t v[4] = { (uint8_t)a0, (uint8_t)a1,
                                         (uint8_t)a2, (uint8_t)a3 };

                        /* Check constraints row by row. */
                        uint8_t sum_above = 0;
                        bool ok = true;
                        for (int i = 0; i < 4; ++i)
                        {
                            uint8_t r1 = (row_sum[s][i] + 1u) & 3u;
                            uint8_t r2 = (sum_above + 2u) & 3u;
                            if (v[i] == r1 || v[i] == r2)
                            {
                                ok = false;
                                break;
                            }
                            sum_above = (sum_above + v[i]) & 3u;
                        }
                        if (!ok)
                            continue;

                        /* Compute new state after adding this column. */
                        int new_s = 0;
                        for (int i = 0; i < 4; ++i)
                        {
                            uint8_t ns = (row_sum[s][i] + v[i]) & 3u;
                            new_s += ns << (2 * i);  /* base‑4 encoding */
                        }

                        trans[s][new_s] += 1;
                    }
    }

    tables_ready = true;
}

/* DP function requested by the harness. */
seq_t a(idx_t n)
{
    build_tables();

    seq_t dp[256]  = {0};
    seq_t newdp[256] = {0};

    /* initial state: all row sums zero */
    dp[0] = (seq_t)1;

    for (idx_t col = 0; col < n; ++col)
    {
        /* clear next column DP */
        for (int i = 0; i < 256; ++i)
            newdp[i] = (seq_t)0;

        /* Transition step */
        for (int s = 0; s < 256; ++s)
        {
            if (dp[s] == (seq_t)0)
                continue;
            for (int t = 0; t < 256; ++t)
            {
                uint16_t cnt = trans[s][t];
                if (cnt == 0)
                    continue;
                seq_t tmp = dp[s] * (seq_t)cnt;
                newdp[t] += tmp;
            }
        }

        /* prepare for next column */
        for (int i = 0; i < 256; ++i)
            dp[i] = newdp[i];
    }

    /* Sum all states to obtain the final count. */
    seq_t ans = (seq_t)0;
    for (int i = 0; i < 256; ++i)
        ans += dp[i];

    return ans;
}
