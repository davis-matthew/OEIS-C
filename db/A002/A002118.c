/*
 * A002118  5th powers written backwards.
 * https://oeis.org/A002118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = reverse_digits(n^5)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 64, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t fifth = n_seq * n_seq * n_seq * n_seq * n_seq;
    
    seq_t result = 0;
    seq_t temp = fifth;
    
    while (temp > 0) {
        result = result * 10 + (temp % 10);
        temp /= 10;
    }
    
    return result;
}
