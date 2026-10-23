/*
 * A125626  Numbers n whose reverse binary representation has the following
 *          property: let a 0 mean "halving" and a 1 mean "k -> 3k+1". The
 *          number describes an operation k -> f_n(k). If the equation f_n(k)
 *          = k has a positive solution, n is a term in the sequence.
 * https://oeis.org/A125626
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = nth integer ≥ 4 such that 3^(#ones) < 2^(#zeros) where
 *            #ones and #zeros are the numbers of 1‑bits and 0‑bits in the
 *            binary representation of the candidate
 * Range:     u8 n <= 48, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true iff x satisfies 3^(#ones) < 2^(#zeros).  */
static bool satisfies(seq_t x)
{
    unsigned int ones = 0;      /* count of 1‑bits */
    unsigned int bitlen = 0;    /* total #bits */

    seq_t t = x;
    while (t)            /* loop while bits remain */
    {
        if ((t & 1) != 0)   /* low bit is 1? */
            ones++;
        t >>= 1;
        bitlen++;
    }

    unsigned int zeros = bitlen - ones;

    /* p3 = 3^ones, p2 = 2^zeros   */
    seq_t p3 = 1;
    for (unsigned int i = 0; i < ones; ++i)
        p3 *= 3;

    seq_t p2 = 1;
    p2 <<= zeros;          /* 2^zeros */

    return p3 < p2;
}

/* Return the nth term of A125626 (OEIS offset 4).  */
seq_t a(idx_t n)
{
    if (n < 4)          /* behaviour below offset is irrelevant */
        return 0;

    idx_t k = n - 3;    /* we need the k‑th satisfying integer */
    idx_t count = 0;
    seq_t m = 4;        /* the first integer to test */

    while (true)
    {
        if (satisfies(m))
        {
            count++;
            if (count == k)
                return m;
        }
        m += 1;          /* next candidate */
    }
}
