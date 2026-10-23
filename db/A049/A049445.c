/*
 * A049445  Numbers k with the property that the number of 1's in binary
 *          expansion of k (see A000120) divides k.
 * https://oeis.org/A049445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, base
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 8931, u32 n <= 9223, u64 n <= 9223,
 *            u128 n <= 9223, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth integer k such that the number of 1's in the binary
 * representation of k (popcount(k)) divides k.
 * The implementation searches sequentially for the required
 * integers; the density is about (2·log 2)/log(k), so for n up
 * to ~10 000 the loop runs only ~1 e5 iterations.
 * No external tables or advanced algorithms are used. */

/* if necessary include stdint.h for typical type definitions */
#include <stdint.h>

/* helper: popcount for arbitrary-sized seq_t, using only
 * integral arithmetic and bitwise operations. */
static inline seq_t popcount_seq_t(seq_t x)
{
    seq_t c = 0;
    while (x != 0)
    {
        c += (x & 1);
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* The function assumes n >= 1; behaviour for n < 1 is undefined. */
    idx_t found = 0;          /* number of valid terms found so far */
    seq_t k = 1;              /* current candidate integer */

    while (1)
    {
        seq_t pc = popcount_seq_t(k);   /* popcount of k */
        /* k > 0, so pc > 0 always; check for safety anyway */
        if (pc != 0 && (k % pc) == 0)
        {
            ++found;
            if (found == n)
                return k;
        }
        ++k;
    }
}
