/*
 * A207748  Number of nX4 0..1 arrays avoiding 0 0 1 and 0 1 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A207748: number of n×4 binary arrays avoiding horizontal 001 and 011,
   and vertical 001 and 100 patterns.
   Computed by a transfer‑matrix DP over the 9 allowed 4‑bit rows:
   0000,0100,0101,1000,1010,1100,1101,1110,1111.
   a(1)=9, a(2)=81, for n≥3 obtained by iterating the DP. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)9;
    if (n == 2)
        return (seq_t)81;

    /* Allowed 4‑bit rows. */
    const unsigned char vals[9] = {0, 4, 5, 8, 10, 12, 13, 14, 15};

    /* Transition table: trans[p][k] == true iff rows (i,j,k) are valid
       where p = i*9 + j, k is index of the new row. */
    static bool trans[81][9];
    static bool init = false;
    if (!init)
    {
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                for (int k = 0; k < 9; ++k)
                {
                    int a = vals[i];
                    int b = vals[j];
                    int c = vals[k];
                    bool ok = true;
                    for (int col = 0; col < 4; ++col)
                    {
                        int bit_a = (a >> col) & 1;
                        int bit_b = (b >> col) & 1;
                        int bit_c = (c >> col) & 1;
                        if ((bit_a == 0 && bit_b == 0 && bit_c == 1) ||
                            (bit_a == 1 && bit_b == 0 && bit_c == 0))
                        {
                            ok = false;
                            break;
                        }
                    }
                    trans[i * 9 + j][k] = ok;
                }
        init = true;
    }

    /* DP over pairs of consecutive rows. */
    seq_t dp[81];
    seq_t nextdp[81];
    /* Initialize counts for the first two rows (no vertical constraints). */
    for (int p = 0; p < 81; ++p)
        dp[p] = (seq_t)0;
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            dp[i * 9 + j] = (seq_t)1;

    /* Build rows 3 … n. */
    for (idx_t row = 3; row <= n; ++row)
    {
        for (int p = 0; p < 81; ++p)
            nextdp[p] = (seq_t)0;

        for (int p = 0; p < 81; ++p)
        {
            seq_t cnt = dp[p];
            if (cnt == 0)
                continue;
            int j = p % 9;          /* second row of the pair */
            for (int k = 0; k < 9; ++k)
                if (trans[p][k])
                {
                    int newp = j * 9 + k;   /* new pair (j,k) */
                    nextdp[newp] += cnt;
                }
        }

        for (int p = 0; p < 81; ++p)
            dp[p] = nextdp[p];
    }

    /* Sum all counts for the last two rows. */
    seq_t total = (seq_t)0;
    for (int p = 0; p < 81; ++p)
        total += dp[p];

    return total;
}
