/*
 * A136441  Number of rotational symmetries in the binary expansion of a
 *          number with 0 appended to the front.
 * https://oeis.org/A136441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of rotational symmetries of the binary representation of n
   with a leading 0. For a binary string S of length L, it returns L/d,
   where d is the minimal period of S. */
#include <stdbool.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* Special case n == 0: S = "00" */
    if (n == 0) {
        return (seq_t)2;  /* L=2, d=1, so a=2 */
    }

    /* Number of bits in n */
    unsigned bits = floor_log2_uint64(n) + 1;    /* >= 1 for n > 0 */
    unsigned L = bits + 1;                      /* length of S = 1 + bits */

    /* Build the binary string S[0..L-1] (as 0/1 chars) */
    unsigned char S[66];  /* L ≤ 65 for 64‑bit n */
    S[0] = 0;             /* prefixed 0 */
    for (unsigned i = 0; i < bits; ++i) {
        unsigned bitpos = bits - 1 - i;          /* msb at position 0 */
        S[1 + i] = (unsigned char)((n >> bitpos) & 1ULL);
    }

    /* Compute KMP prefix (failure) function pi[0..L-1] */
    unsigned pi[66];
    pi[0] = 0;
    for (unsigned i = 1; i < L; ++i) {
        unsigned j = pi[i - 1];
        while (j > 0 && S[i] != S[j]) {
            j = pi[j - 1];
        }
        if (S[i] == S[j]) {
            ++j;
        }
        pi[i] = j;
    }

    unsigned d = L - pi[L - 1];
    if (L % d != 0) {
        d = L;  /* no smaller period */
    }

    seq_t result = ((seq_t)L) / (seq_t)d;
    return result;
}
