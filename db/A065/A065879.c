/*
 * A065879  a(n) is the smallest positive number that is n times the number of
 *          1's in its binary expansion, or 0 if no such number exists.
 * https://oeis.org/A065879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 6138, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): the smallest positive integer m such that m = n * popcount(m).  
   If no such m exists returns 0.  The implementation searches over the
   possible number of ones k, computing m = n * k and checking whether
   popcount(m) == k.  The search stops when k exceeds the bit length of
   m, because k cannot be larger than the number of bits in m. */

#include <stdint.h>

/* Count the ones in the binary representation of a `seq_t` value. */
static seq_t popcount_seq(seq_t x)
{
    seq_t cnt = 0;
    while (x) {
        cnt += x & 1;
        x >>= 1;
    }
    return cnt;
}

/* Return the bit length of a `seq_t` value (number of bits needed to
   represent it; zero would return 0, but this function is only called
   with positive values). */
static unsigned bitlen_seq_t(seq_t x)
{
    unsigned len = 0;
    while (x) {
        len++;
        x >>= 1;
    }
    return len;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* indices start at 1, but guard anyway */

    seq_t n_seq = (seq_t)n;
    for (idx_t k = 1; ; ++k) {
        seq_t m = n_seq * (seq_t)k;
        seq_t pc = popcount_seq(m);
        if (pc == (seq_t)k) return m;

        unsigned bits = bitlen_seq_t(m);
        if ((idx_t)k > bits) break;   /* impossible for larger k */
    }
    return 0;
}
