/*
 * A202902  Number of n X 4 0..1 arrays with every one equal to some NW, E or
 *          S neighbor.
 * https://oeis.org/A202902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of n×4 0..1 matrices in which every 1 has a * 1‐neighbor
 *            to the north‑west (NW), east (E) or south (S). * The
 *            implementation uses a dynamic programming over the last * two
 *            rows; it works for any seq_t (integer of any fixed size * or an
 *            arbitrary‑precision class) and any idx_t. It runs in *...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Check if every 1 in `prev` has a 1–neighbor among
 * NW (in prevprev), E (in prev at column+1), or S (in next). */
static inline bool row_satisfied(uint8_t prevprev, uint8_t prev, uint8_t next)
{
    for (uint8_t c = 0; c < 4; ++c)
    {
        if ((prev >> c) & 1)
        {
            bool ok = false;
            /* NW */
            if (c > 0 && ((prevprev >> (c - 1)) & 1))
                ok = true;
            /* E */
            if (!ok && c < 3 && ((prev >> (c + 1)) & 1))
                ok = true;
            /* S */
            if (!ok && ((next >> c) & 1))
                ok = true;
            if (!ok)
                return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;    /* offset is 1; n < 1 is unused */

    /* States are encoded as:  state = (prevprev << 4) | prev  (both 4‑bit) */
    static seq_t dp[256];
    static seq_t next[256];

    /* clear dp */
    for (size_t i = 0; i < 256; ++i)
        dp[i] = 0;

    /* first row: prevprev == 0 */
    for (uint8_t r = 0; r < 16; ++r)
        dp[r] = 1;   /* state (0, r) */

    /* Process rows 2..n */
    for (idx_t row = 2; row <= n; ++row)
    {
        for (size_t i = 0; i < 256; ++i)
            next[i] = 0;

        for (size_t idx = 0; idx < 256; ++idx)
        {
            seq_t val = dp[idx];
            if (val == 0)
                continue;
            uint8_t prevprev = (uint8_t)(idx >> 4);
            uint8_t prev     = (uint8_t)(idx & 15);

            for (uint8_t nxt = 0; nxt < 16; ++nxt)
            {
                if (row_satisfied(prevprev, prev, nxt))
                {
                    size_t newidx = ((size_t)prev << 4) | nxt;
                    next[newidx] += val;
                }
            }
        }

        /* swap dp and next */
        for (size_t i = 0; i < 256; ++i)
            dp[i] = next[i];
    }

    /* Final check: last row must also satisfy */
    seq_t ans = 0;
    for (size_t idx = 0; idx < 256; ++idx)
    {
        seq_t val = dp[idx];
        if (val == 0)
            continue;
        uint8_t prevprev = (uint8_t)(idx >> 4);
        uint8_t prev     = (uint8_t)(idx & 15);
        if (row_satisfied(prevprev, prev, 0))
            ans += val;
    }

    return ans;
}
