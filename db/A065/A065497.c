/*
 * A065497  Number of (binary) bit strings of length n having at least one
 *          even length block of 0's followed by an even length block of 1's.
 * https://oeis.org/A065497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 32, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A065497: Number of binary bit strings of length n having at least one
 * even-length block of 0's immediately followed by an even-length block of 1's.
 * This implementation counts the strings that avoid the forbidden pattern
 * using a 6‑state deterministic automaton; the desired count is
 *     a(n) = 2^n - avoided(n).
 * Here avoided(n) is the number of length‑n strings that never contain
 * a substring 0^{2k}1^{2m} (k,m ≥ 1).  
 */

/* Return the n-th term of sequence A065497. */
seq_t a(idx_t n)
{
    /* For n = 0 the answer is 0 (empty string has no such block). */
    if (n == 0)
        return 0;

    /* DP states:
     * 0: zeros run, odd length (Z_odd)
     * 1: zeros run, even length (Z_even)
     * 2: ones run, odd length, preceding zeros odd (O_odd_Z_odd)
     * 3: ones run, odd length, preceding zeros even (O_odd_Z_even)
     * 4: ones run, even length, preceding zeros odd  (O_even_Z_odd)
     * 5: ones run, even length, preceding zeros even (O_even_Z_even)
     * State 5 cannot be a final state: any string finishing in state 5
     * contains the forbidden pattern, so it is excluded from the avoided count.
     */
    seq_t dp[6]   = {0, 0, 0, 0, 0, 0};
    seq_t nxt[6]  = {0, 0, 0, 0, 0, 0};

    /* Length 1 initialization */
    dp[0] = 1;   /* "0"   → Z_odd      */
    dp[2] = 1;   /* "1"   → O_odd_Z_odd  */

    /* total strings of length i: start with 2^1 = 2 */
    seq_t total = 2;

    for (idx_t i = 2; i <= n; ++i) {
        /* reset next layer */
        for (int s = 0; s < 6; ++s)
            nxt[s] = 0;

        for (int s = 0; s < 6; ++s) {
            seq_t val = dp[s];
            if (!val) continue;

            switch (s) {
            case 0: /* Z_odd */
                /* add 0 → Z_even */
                nxt[1] += val;
                /* add 1 → O_odd_Z_odd */
                nxt[2] += val;
                break;
            case 1: /* Z_even */
                /* add 0 → Z_odd */
                nxt[0] += val;
                /* add 1 → O_odd_Z_even */
                nxt[3] += val;
                break;
            case 2: /* O_odd_Z_odd */
                /* add 1 → O_even_Z_odd */
                nxt[4] += val;
                /* add 0 → Z_odd */
                nxt[0] += val;
                break;
            case 3: /* O_odd_Z_even */
                /* add 1 → O_even_Z_even (forbidden zone) */
                nxt[5] += val;
                /* add 0 → Z_odd */
                nxt[0] += val;
                break;
            case 4: /* O_even_Z_odd */
                /* add 1 → O_odd_Z_odd */
                nxt[2] += val;
                /* add 0 → Z_odd */
                nxt[0] += val;
                break;
            case 5: /* O_even_Z_even (cannot end the string) */
                /* only way is to add 1 (continue the even-ones run) */
                nxt[3] += val;
                /* adding 0 would finish with even ones after even zeros → forbidden */
                break;
            }
        }

        /* copy next layer back to dp */
        for (int s = 0; s < 6; ++s)
            dp[s] = nxt[s];

        total *= 2;                  /* total strings of length i */
    }

    /* Count avoided strings: all final states except 5 */
    seq_t avoided = dp[0] + dp[1] + dp[2] + dp[3] + dp[4];

    return total - avoided;
}
