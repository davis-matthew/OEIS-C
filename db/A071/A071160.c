/*
 * A071160  Łukasiewicz words that are also valid asynchronic siteswap
 *          juggling patterns.
 * https://oeis.org/A071160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) is obtained from the binary expansion of n as follows.
 *            Starting from the most significant bit, each 1‑bit is replaced
 *            by the cyclic distance to the next 1‑bit to the right (wrapping
 *            from the least significant 1‑bit to the most significant 1‑bit).
 *            0‑bits become zero. The resulting sequence of...
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case: n == 0 */
    if (n == 0) return 0;

    /* 1.  Find bit length of n */
    int len = 0;
    idx_t tmp = n;
    while (tmp != 0) {
        ++len;
        tmp >>= 1;
    }

    /* 2.  Store positions of set bits (ascending from least‑significant). */
    int ones[64];           /* 64 is enough for any 64‑bit integer */
    int ones_cnt = 0;
    for (int i = 0; i < len; ++i) {
        if (n & ( (idx_t)1 << i ))
            ones[ones_cnt++] = i;
    }

    /* 3.  Most significant 1‑bit index */
    int msb_index = ones[ones_cnt - 1];

    /* 4.  Build the decimal number digit by digit, from most significant
           bit to least significant bit. */
    seq_t result = 0;

    for (int i = len - 1; i >= 0; --i) {
        seq_t digit = 0;
        if ((n >> i) & 1U) { /* i‑th bit is 1 */
            /* Find next 1‑bit to the right (with wrap‑around). */
            int next = msb_index;              /* default wrap */
            for (int t = ones_cnt - 1; t >= 0; --t) {
                if (ones[t] < i) {             /* first lower 1‑bit found */
                    next = ones[t];
                    break;
                }
            }
            /* Compute cyclic distance to the next 1‑bit. */
            int dist;
            if (i > next)
                dist = i - next;
            else
                dist = i - next + len;          /* i == next → only one 1‑bit */
            digit = (seq_t)dist;
        }
        result = result * 10 + digit;
    }

    return result;
}
