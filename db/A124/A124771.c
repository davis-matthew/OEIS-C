/*
 * A124771  Number of distinct subsequences for compositions in standard
 *          order.
 * https://oeis.org/A124771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf, base
 * Formula:   a(n) = number of distinct c‑divisors of n in binary (i.e. number
 *            of distinct numbers obtained by concatenating contiguous blocks
 *            of the form 10…0 that partition the binary representation of n)
 * Range:     u8 n <= 1053, u16 n <= 9936, u32 n <= 9936, u64 n <= 9936,
 *            u128 n <= 9936, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* a(0) = 1 (only the empty subsequence) */
    if (n == 0) return (seq_t)1;

    /* Collect positions of set bits from most significant to least. */
    int pos[64];
    int k = 0;
    for (int i = 63; i >= 0; --i) {
        if ((n >> i) & 1) {
            pos[k++] = i;
        }
    }

    /* Parts are powers of two. Compute their values and the bit
       lengths of the following parts (needed for concatenation). */
    seq_t parts[64];
    int shift_len[64];          /* bits of the part = exponent + 1 */
    for (int j = 0; j < k; ++j) {
        if (j < k - 1) {
            int len = pos[j] - pos[j + 1];        /* zeros between this 1 and the next 1 */
            shift_len[j] = len;                   /* exponent = len-1, so bits = len */
            parts[j] = ((seq_t)1 << (shift_len[j] - 1));
        } else {                                 /* last part */
            shift_len[j] = pos[j] + 1;            /* exponent = pos[j], bits = pos[j]+1 */
            parts[j] = ((seq_t)1 << pos[j]);
        }
    }

    /* Count distinct concatenations of contiguous subarrays of parts. */
    const size_t MAX_UNIQ = 2080;                /* 64 * 65 / 2 */
    seq_t uniq[MAX_UNIQ];
    size_t uniq_len = 0;

    for (int i = 0; i < k; ++i) {
        seq_t cur = parts[i];
        bool found = false;
        for (size_t u = 0; u < uniq_len; ++u) {
            if (cur == uniq[u]) { found = true; break; }
        }
        if (!found) uniq[uniq_len++] = cur;

        for (int j = i + 1; j < k; ++j) {
            cur = (cur << shift_len[j]) | parts[j];
            found = false;
            for (size_t u = 0; u < uniq_len; ++u) {
                if (cur == uniq[u]) { found = true; break; }
            }
            if (!found) uniq[uniq_len++] = cur;
        }
    }

    return (seq_t)(uniq_len + 1);  /* add the empty subsequence */
}
