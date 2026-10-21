/*
 * A202312  Number of (n+2)X5 binary arrays avoiding patterns 001 and 100 in
 *          rows and columns.
 * https://oeis.org/A202312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×5 binary arrays avoiding patterns 001 and
 *            100 in rows and columns
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the value for any n >= 1.  It uses dynamic programming with a state
   consisting of the last two columns of the array.  The state space is 2^(5+5)=1024,
   and the transition is pre‑computed once to speed up repeated calls. */

#include <stdint.h>
#include <stdbool.h>

/* define constants */
#ifdef __GNUC__
#  define CONSTEXPR static const
#else
#  define CONSTEXPR const
#endif

#ifndef STATES
#  define STATES   (1 << 10)   /* 32 columns * 32 columns = 1024 states */
#endif
#ifndef COLUMNS
#  define COLUMNS  32
#endif

seq_t a(idx_t n)
{
    const idx_t m = n + (idx_t)2;   /* number of columns */

    /* pre-computed data, initialized once */
    static bool init = false;
    static bool allowed[COLUMNS];
    static uint32_t trans_mask[STATES];
    if (!init)
    {
        /* Which single columns are allowed (vertical patterns). */
        for (int col = 0; col < COLUMNS; ++col)
        {
            bool ok = true;
            for (int i = 0; i < 3; ++i)
            {
                int a = (col >> i)       & 1;
                int b = (col >> (i + 1)) & 1;
                int c = (col >> (i + 2)) & 1;
                if ((a == 0 && b == 0 && c == 1) ||
                    (a == 1 && b == 0 && c == 0))
                {
                    ok = false;
                    break;
                }
            }
            allowed[col] = ok;
        }

        /* pre-compute horizontal-valid transition mask for each state. */
        for (int s = 0; s < STATES; ++s)
        {
            uint32_t mask = 0;
            int prev1 = s & 31;          /* bits 0..4 : last column */
            for (int col = 0; col < COLUMNS; ++col)
            {
                if (!allowed[col]) continue;

                bool ok = true;
                /* prev2 bits are bits 5..9 of the state */
                for (int r = 0; r < 5; ++r)
                {
                    int a = (s >> (5 + r)) & 1;  /* prev2 bit */
                    int b = (prev1 >> r)        & 1;
                    int d = (col   >> r)        & 1;
                    if ((a == 0 && b == 0 && d == 1) ||
                        (a == 1 && b == 0 && d == 0))
                    {
                        ok = false;
                        break;
                    }
                }
                if (ok) mask |= (1u << col);
            }
            trans_mask[s] = mask;
        }
        init = true;
    }

    static seq_t dp[STATES];
    static seq_t ndp[STATES];

    /* initialize DP for 0 columns placed */
    for (int i = 0; i < STATES; ++i) dp[i] = 0;
    dp[0] = 1;  /* state prev1=0, prev2=0 */

    /* perform DP over all columns */
    for (idx_t idx = 0; idx < m; ++idx)
    {
        for (int i = 0; i < STATES; ++i) ndp[i] = 0;

        if (idx <= 1)
        {
            /* first or second column: no horizontal check */
            for (int state = 0; state < STATES; ++state)
            {
                seq_t count = dp[state];
                if (count == 0) continue;
                int prev1 = state & 31;   /* last column */
                for (int col = 0; col < COLUMNS; ++col)
                {
                    if (!allowed[col]) continue;
                    int new_state = (col) | (prev1 << 5);
                    ndp[new_state] += count;
                }
            }
        }
        else
        {
            /* later columns: check horizontal patterns */
            for (int state = 0; state < STATES; ++state)
            {
                seq_t count = dp[state];
                if (count == 0) continue;
                int prev1 = state & 31;
                uint32_t mask = trans_mask[state];
                for (int col = 0; col < COLUMNS; ++col)
                {
                    if (!(mask & (1u << col))) continue;
                    int new_state = (col) | (prev1 << 5);
                    ndp[new_state] += count;
                }
            }
        }

        /* swap dp and ndp */
        for (int i = 0; i < STATES; ++i) dp[i] = ndp[i];
    }

    /* sum all ending states */
    seq_t total = 0;
    for (int i = 0; i < STATES; ++i) total += dp[i];
    return total;
}
