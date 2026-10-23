/*
 * A153033  Numbers with adjacent 1's and no adjacent 0's in binary expansion.
 * https://oeis.org/A153033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the nth integer whose binary representation contains at
 *            least one occurrence of "11" and does not contain "00"
 *            substrings
 * Range:     u8 n <= 79, u16 n <= 4163, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* DP table: dp[rem][prev][has] = number of ways to fill rem bits given previous bit = prev
   and whether an adjacent pair of 1's has already appeared = has.  rem is the number of
   bits still to be placed (including the current bit).  The table is built once for a
   fixed upper bound on the remaining length.  The counts are stored as 64‑bit unsigned
   integers because the required n is small (≤ 10000) and the DP values stay far below
   2^64 for the needed lengths. */
static uint64_t dp[71][2][2];
static bool dp_inited = false;

static void init_dp(void)
{
    if (dp_inited) return;
    const int MAX_REM = 70;

    /* Base case: no bits remain.  The completed string is valid iff it already has a
       pair of adjacent 1's. */
    for (int prev = 0; prev <= 1; ++prev)
        for (int has = 0; has <= 1; ++has)
            dp[0][prev][has] = has ? 1ULL : 0ULL;

    /* Build the table for all remaining lengths. */
    for (int rem = 1; rem <= MAX_REM; ++rem)
        for (int prev = 0; prev <= 1; ++prev)
            for (int has = 0; has <= 1; ++has) {
                uint64_t sum = 0;
                if (prev == 0) {
                    /* Only a '1' may follow a '0' to avoid "00". */
                    sum += dp[rem - 1][1][has];
                } else {
                    /* Prev == 1: we may place 0 or 1. */
                    sum += dp[rem - 1][0][has];          /* place 0, has unchanged */
                    sum += dp[rem - 1][1][has | 1];      /* place 1, has becomes 1 */
                }
                dp[rem][prev][has] = sum;
            }

    dp_inited = true;
}

/*--------------------------------------------------------------------
 *  a(n)
 *--------------------------------------------------------------------
 * Computes the nth term of sequence A153033.  The function works for
 * any build where `seq_t` and `idx_t` are integer types (8, 16, 32,
 * 64, 128 bits or an arbitrary‑precision integer type).  It uses a
 * combinatorial DP to count admissible binary strings and then
 * constructs the target string digit by digit.
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n = 0, guard against underflow */

    init_dp();

    idx_t cumulative = 0;      /* sum of counts of vectors of smaller lengths */
    int  L = 1;                /* current length being examined */

    /* Find the length L of the nth admissible number. */
    while (1) {
        uint64_t count_len = (uint64_t)dp[L - 1][1][0];   /* strings of length L */
        if (cumulative + count_len >= n) break;
        cumulative += count_len;
        ++L;
        /* If the length exceeds the DP table, stop (should not happen for n ≤ 10000). */
        if (L - 1 > 70) break;
    }

    /* 0‑based index within the block of strings of length L. */
    uint64_t k = (uint64_t)(n - cumulative - 1);

    /* Build the number bit by bit. */
    seq_t  result = 0;
    int    prev = 1;       /* the first bit is always 1 */
    int    has_adj = 0;    /* have we seen an adjacent pair of 1's yet? */
    result = (result << 1) | 1;   /* set the leading 1 */

    int rem = L - 1;      /* remaining bits to choose */

    while (rem > 0) {
        bool chosen = false;

        /* Try a 0 if it is allowed (i.e. prev == 1). */
        if (prev == 1) {
            int  b = 0;
            int  new_has = has_adj;                /* placing 0 cannot create a new "11" */
            uint64_t cnt = dp[rem - 1][0][new_has];
            if (k < cnt) {
                result = (result << 1) | b;
                prev = b;
                has_adj = new_has;
                --rem;
                chosen = true;
            } else {
                k -= cnt;
            }
        }

        /* If we haven't chosen a 0, try 1.  A 1 is always allowed. */
        if (!chosen) {
            int  b = 1;
            int  new_has = has_adj | ((prev == 1) ? 1 : 0);
            uint64_t cnt = dp[rem - 1][1][new_has];
            /* k must be less than cnt here. */
            result = (result << 1) | b;
            prev = b;
            has_adj = new_has;
            --rem;
        }
    }

    return result;
}
