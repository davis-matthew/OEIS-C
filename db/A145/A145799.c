/*
 * A145799  a(n) = the largest integer that is an (odd) palindrome when
 *          represented in binary and that occurs in the binary representation
 *          of n.
 * https://oeis.org/A145799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 9628, u32 n <= 9628, u64 n <= 9628,
 *            u128 n <= 9628, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the largest odd binary palindrome that appears in n */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    uint64_t val = (uint64_t)n;             /* n fits in 64 bits */
    /* number of bits of n */
    int L = 0;
    uint64_t tmp = val;
    while (tmp) { L++; tmp >>= 1; }
    if (L == 0) return (seq_t)0;            /* should not happen for n>=1 */

    /* bits[0] is the most significant bit */
    int bits[64];
    for (int i = 0; i < L; ++i)
        bits[i] = (int)((val >> (L - 1 - i)) & 1ULL);

    seq_t max_val = 0;

    for (int i = 0; i < L; ++i) {
        if (bits[i] == 0) continue;         /* start must be 1 */
        for (int j = i; j < L; ++j) {
            if (bits[j] == 0) continue;     /* end must be 1 */

            /* check palindrome */
            int l = i, r = j;
            bool ok = true;
            while (l < r) {
                if (bits[l] != bits[r]) { ok = false; break; }
                ++l; --r;
            }
            if (!ok) continue;

            /* value of bits[i..j] */
            int len = j - i + 1;
            uint64_t mask = (len == 64) ? ~0ULL : ((1ULL << len) - 1);
            int shift = L - 1 - j;
            uint64_t subval = (val >> shift) & mask;

            if ((seq_t)subval > max_val) max_val = (seq_t)subval;
        }
    }

    return max_val;
}
