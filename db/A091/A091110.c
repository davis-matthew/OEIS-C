/*
 * A091110  Number of editing steps (deletion, insertion, or substitution) to
 *          transform the binary representation of n into the ternary
 *          representation of n.
 * https://oeis.org/A091110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = LevenshteinDistance( binary(n), ternary(n) )
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline void reverse(uint8_t *arr, int len)
{
    for (int i = 0; i < len / 2; ++i) {
        uint8_t t = arr[i];
        arr[i] = arr[len - 1 - i];
        arr[len - 1 - i] = t;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                            /* never happens for the sequence */

    /* 1. Build the binary digits of n                                 */
    uint8_t bin[64];    /* must be enough for a 64‑bit integer */
    int len_bin = 0;
    idx_t t = n;
    while (t) {
        bin[len_bin++] = (uint8_t)(t & 1);
        t >>= 1;
    }
    reverse(bin, len_bin);

    /* 2. Build the ternary digits of n                                 */
    uint8_t ter[64];
    int len_ter = 0;
    t = n;
    while (t) {
        ter[len_ter++] = (uint8_t)(t % 3);
        t /= 3;
    }
    reverse(ter, len_ter);

    /* 3. Levenshtein dynamic programming with two rows                */
    seq_t prev[65];
    seq_t cur[65];

    for (int j = 0; j <= len_ter; ++j)
        prev[j] = (seq_t)j;                     /* distance from empty bin to first j ter digits */

    for (int i = 1; i <= len_bin; ++i) {
        cur[0] = (seq_t)i;                       /* distance from first i bin digits to empty ter */

        for (int j = 1; j <= len_ter; ++j) {
            seq_t cost = (bin[i - 1] == ter[j - 1]) ? 0 : 1;
            seq_t del = prev[j] + 1;             /* delete from bin */
            seq_t ins = cur[j - 1] + 1;           /* insert to match ter */
            seq_t sub = prev[j - 1] + cost;      /* substitute */
            seq_t best = del;
            if (ins < best) best = ins;
            if (sub < best) best = sub;
            cur[j] = best;
        }

        /* copy cur into prev for the next iteration                  */
        for (int j = 0; j <= len_ter; ++j)
            prev[j] = cur[j];
    }

    return prev[len_ter];
}
