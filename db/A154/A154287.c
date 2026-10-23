/*
 * A154287  (L)-sieve transform of {1,4,9,16,...,n^2,...}=A000290.
 * https://oeis.org/A154287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>1, a(n)=a(n-1)+v where the sequence v follows
 *            2,3,3,4,5,5,6,6,7,7,8,9,9,... i.e. each integer m≥2 appears
 *            twice except powers of 2 which appear only once
 * Range:     u8 n <= 27, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= (idx_t)1) {
        return (seq_t)1;
    }
    seq_t sum = (seq_t)1;
    idx_t v = (idx_t)2;      /* current difference value */
    unsigned rem = 1;        /* remaining copies of current v (1 for powers of 2, 2 otherwise) */

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        sum += (seq_t)v;
        if (--rem == 0) {
            ++v;
            rem = ((v & (v - (idx_t)1)) == (idx_t)0) ? 1 : 2;
        }
    }
    return sum;
}
