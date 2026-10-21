/*
 * A213724  Largest natural number x such that x = n + A000120(x), or zero if
 *          no such number exists.
 * https://oeis.org/A213724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest natural number x such that x = n + popcount(x), *
 *            where popcount(x) is the number of 1‑bits of x. Returns 0 if no
 *            such x exists
 * Range:     u8 n <= 254, u16 n <= 1024, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <limits.h>  /* for CHAR_BIT */

static seq_t seq_popcount(seq_t x)
{
    seq_t c = 0;
    while (x) {
        x &= x - 1;    /* drop lowest set bit */
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;

    /* Upper bound for possible popcount values: at most 128 bits of x. */
    int maxbits = sizeof (seq_t) * CHAR_BIT;
    if (maxbits > 128) maxbits = 128;

    for (int k = maxbits; k >= 0; --k) {
        seq_t kseq = (seq_t)k;
        seq_t x = N + kseq;
        if (x < N)          /* overflow of unsigned addition – skip this k */
            continue;
        if (seq_popcount(x) == kseq)
            return x;
    }
    return 0;
}
