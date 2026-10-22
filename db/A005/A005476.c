/*
 * A005476  a(n) = n*(5*n - 1)/2.
 * https://oeis.org/A005476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(5*n - 1)/2
 * Range:     u8 n <= 10, u16 n <= 162, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t five_n_minus_1 = 5 * n_seq - 1;
    
    if (n % 2 == 0) {
        return (n_seq / 2) * five_n_minus_1;
    } else {
        return n_seq * (five_n_minus_1 / 2);
    }
}
