/*
 * A080277  Partial sums of A038712.
 * https://oeis.org/A080277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = \sum_{k=0}^{\lfloor\log_2 n\rfloor} 2^k * \lfloor n/2^k
 *            \rfloor (recurrence: a(n) = n + 2*a(floor(n/2)), a(1)=1)
 * Range:     u8 n <= 47, u16 n <= 5247, u32 n <= 9902, u64 n <= 9902,
 *            u128 n <= 9902, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t v = n;
    unsigned shift = 0;
    while (v > 0) {
        sum += ((seq_t)v) << shift;
        v >>= 1;
        shift++;
    }
    return sum;
}
