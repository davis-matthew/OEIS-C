/*
 * A234005  Number of binary words of length n avoiding the subword given by
 *          the binary expansion of n.
 * https://oeis.org/A234005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binary words of length n that avoid the *
 *            subword given by the binary representation of n. * * For each n
 *            > 0 the forbidden pattern P is the binary expansion of n *
 *            (without leading zeros). The count equals the number of walks of
 *            * length n in the KMP automaton for pattern P that...
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the empty word */
    if (n == 0)
        return (seq_t)1;

    /* Compute the length of the binary pattern P = binary(n) */
    idx_t L = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        ++L;
        tmp >>= 1;
    }

    /* Store the pattern bits: pat[0] = most‑significant bit, pat[L-1] = least */
    uint8_t pat[32] = {0};          /* 32 > log2(10000) */
    for (idx_t i = 0; i < L; ++i)
        pat[L - 1 - i] = (n >> i) & 1U;

    /* Transition table: trans[s][b] = next state (0…L‑1) or -1 for forbidden */
    int trans[32][2];
    for (idx_t s = 0; s < L; ++s) {
        for (int b = 0; b <= 1; ++b) {
            int t;                        /* next state */
            for (t = (int)s + 1; t >= 0; --t) {
                /* Check if pattern[0..t-1] is suffix of (pat[0..s-1] + b) */
                int matched = 1;
                for (idx_t j = 0; j < (idx_t)t; ++j) {
                    int offset = (int)(s + 1) - t + (int)j; // position in candidate
                    int cand_bit;
                    if (offset < (int)s)
                        cand_bit = pat[offset];
                    else if (offset == (int)s)
                        cand_bit = b;
                    else
                        cand_bit = 0; /* unreachable */
                    if (pat[j] != cand_bit) {
                        matched = 0;
                        break;
                    }
                }
                if (matched) break;       /* t is the longest possible suffix length */
            }
            if (t == (int)L) /* full match – forbidden state */
                trans[s][b] = -1;
            else
                trans[s][b] = t;
        }
    }

    /* DP arrays */
    seq_t dp[32]  = {0};
    seq_t ndp[32] = {0};
    dp[0] = (seq_t)1;          /* start in state 0 */

    for (idx_t step = 0; step < n; ++step) {
        /* Zero ndp */
        for (idx_t i = 0; i < L; ++i) ndp[i] = (seq_t)0;

        for (idx_t s = 0; s < L; ++s) if (dp[s] != (seq_t)0) {
            for (int b = 0; b <= 1; ++b) {
                int ns = trans[s][b];
                if (ns >= 0)
                    ndp[ns] += dp[s];
            }
        }
        /* Copy ndp to dp for next iteration */
        for (idx_t i = 0; i < L; ++i) dp[i] = ndp[i];
    }

    /* Sum all non‑forbidden states */
    seq_t result = (seq_t)0;
    for (idx_t i = 0; i < L; ++i)
        result += dp[i];

    return result;
}
