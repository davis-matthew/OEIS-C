/*
 * A092038  a(n+1) = a(n) + (a(n) mod 2)^(n mod a(n)), a(1) = 1.
 * https://oeis.org/A092038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/4)+floor((n-1)/4)+2 for n>1; a(1)=1
 * Range:     u8 n <= 456, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }
    seq_t k = (seq_t)n;
    seq_t v1 = k / (seq_t)4;
    seq_t v2 = (k - (seq_t)1) / (seq_t)4;
    return v1 + v2 + (seq_t)2;
}
