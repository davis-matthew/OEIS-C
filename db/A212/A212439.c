/*
 * A212439  a(n) = 2*n + A181935(n) mod 2.
 * https://oeis.org/A212439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2*n + parity_of_curling_number(n)
 * Range:     u8 n <= 127, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where parity_of_curling_number(n) is A181935(n) mod 2 */

#include <stdint.h>

/* Helper: return bit length of n, treating 0 as length 1. */
static inline int bitlen_uint64(idx_t n)
{
    if (n == 0) return 1;
    int len = 0;
    while (n > 0) {
        len++;
        n >>= 1;
    }
    return len;
}

/* Compute parity (0 or 1) of the curling number of the binary
   representation of n.  The algorithm is O(L³) with L = bit length,
   but L ≤ 64 for n < 2⁶⁴, so it is trivial in speed. */
static inline seq_t parity_of_curling(idx_t n)
{
    int L = bitlen_uint64(n);
    unsigned char bits[64];             /* L ≤ 64 */
    /* Build most‑significant‑bit first */
    for (int i = L - 1; i >= 0; --i)
        bits[L - 1 - i] = (n >> i) & 1;

    int best = 1;      /* curling number is at least 1 */

    /* Test each suffix of the binary string */
    for (int start = 0; start < L; ++start) {
        int suffix_len = L - start;
        int period = suffix_len;        /* default: whole suffix */

        /* Find minimal period of this suffix */
        for (int m = 1; m <= suffix_len; ++m) {
            if (suffix_len % m != 0) continue;   /* must divide length */

            int k = suffix_len / m;
            int i, ok = 1;
            for (i = 0; i < suffix_len; ++i) {
                if (bits[start + i] != bits[start + i % m]) {
                    ok = 0;
                    break;
                }
            }
            if (ok) {
                period = m;
                break;      /* minimal period found */
            }
        }
        int reps = suffix_len / period;
        if (reps > best) best = reps;
    }
    return (seq_t)(best & 1);   /* parity of best */
}

/* Sequence function required by the test harness */
seq_t a(idx_t n)
{
    seq_t parity = parity_of_curling(n);
    return ((seq_t)n << 1) + parity;
}
