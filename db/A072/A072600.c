/*
 * A072600  Numbers which in base 2 have fewer 0's than 1's.
 * https://oeis.org/A072600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 142, u16 n <= 4733, u32 n <= 4733, u64 n <= 4733,
 *            u128 n <= 4733, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose binary representation contains fewer 0's than 1's.
   a(n) = the n-th positive integer m such that popcount(m) > bitlen(m)-popcount(m).
   The sequence is enumerated in increasing order of m.  No caching is used; the
   function simply scans successive integers until the n‑th qualifying value
   is found.  This works for all supported integer types, including arbitrary‑
   precision classes. */
#include <stdbool.h>

/* Count the number of 1 bits in a seq_t value. */
static int popcount_seq(seq_t v) {
    int cnt = 0;
    while (v) {
        cnt += (int)(v & 1);
        v >>= 1;
    }
    return cnt;
}

/* Return the number of bits needed to represent a seq_t value
   (the position of the highest set bit + 1). */
static int bitlen_seq(seq_t v) {
    int len = 0;
    while (v) {
        len++;
        v >>= 1;
    }
    return len;
}

/* Return the nth term of A072600 (1‑based index). */
seq_t a(idx_t n) {
    seq_t m = (seq_t)1;          /* candidate integer to test */
    idx_t found = 0;             /* how many qualifying numbers found */

    while (1) {
        int ones = popcount_seq(m);
        int bits = bitlen_seq(m);
        if (bits - ones < ones) {    /* fewer zeros than ones */
            found++;
            if (found == n)
                return m;
        }
        m++;                          /* go to next integer */
    }
}
