/*
 * A239826  Sum of divisors of n that are also suffixes of n in binary
 *          representation.
 * https://oeis.org/A239826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = sum of all divisors d of n such that
   the binary representation of d is a suffix of the binary
   representation of n.  This is implemented by enumerating all
   divisors of n and testing the suffix property. */
static inline unsigned bitlen_u64(idx_t x)
{   unsigned l = 0; while (x) { ++l; x >>= 1; } return l; }

static inline seq_t suffix_mask(idx_t d)
{
    unsigned len = bitlen_u64(d);
    unsigned bits_seq = sizeof(seq_t) * 8;
    if (len == bits_seq) return ~((seq_t)0);      /* all bits set */
    return (((seq_t)1) << len) - 1;               /* len low bits set */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            idx_t d1 = i;
            idx_t d2 = n / i;

            seq_t mask1 = suffix_mask(d1);
            if (((seq_t)n & mask1) == (seq_t)d1) sum += (seq_t)d1;

            if (d2 != d1)
            {
                seq_t mask2 = suffix_mask(d2);
                if (((seq_t)n & mask2) == (seq_t)d2) sum += (seq_t)d2;
            }
        }
    }
    return sum;
}
