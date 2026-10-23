/*
 * A008576  Coordination sequence for planar net 4.8.8.
 * https://oeis.org/A008576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n>0, a(3k)=8k, a(3k+1)=8k+3, a(3k+2)=8k+5
 * Range:     u8 n <= 95, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    idx_t k = n / (idx_t)3;
    switch (n % (idx_t)3) {
        case 0:
            return (seq_t)8 * (seq_t)k;
        case 1:
            return (seq_t)8 * (seq_t)k + (seq_t)3;
        case 2:
            return (seq_t)8 * (seq_t)k + (seq_t)5;
    }
    /* unreachable but keeps compiler happy */
    return (seq_t)0;
}
