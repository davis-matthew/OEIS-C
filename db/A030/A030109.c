/*
 * A030109  Write n in binary, reverse bits, subtract 1, divide by 2.
 * https://oeis.org/A030109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf, look
 * Formula:   a(n) = (rev(n) - 1) / 2 where rev(n) is the integer obtained by
 *            writing n in binary, with exactly its significant bits,
 *            reversing that bit string, and interpreting the result again as
 *            a binary number
 * Range:     u8 n <= 512, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Count the number of bits required to represent n (n > 0). */
static unsigned bitlen_idx(idx_t n)
{
    unsigned len = 0;
    while (n) {
        ++len;
        n >>= 1;
    }
    return len;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* the sequence is defined for n ≥ 1; return 0 for safety */
        return (seq_t)0;

    unsigned k = bitlen_idx(n);       /* number of significant bits of n */

    /* Reverse the bits of n.  Use an idx_t accumulator so we do not
       overflow seq_t while building the value. */
    idx_t rev = 0, tmp = n;
    for (unsigned i = 0; i < k; ++i) {
        rev = (rev << 1) | (tmp & 1);
        tmp >>= 1;
    }

    /* rev ≥ 1 for all n ≥ 1, so rev‑1 is safe. */
    return (seq_t)((rev - 1) >> 1);
}
