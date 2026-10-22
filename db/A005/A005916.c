/*
 * A005916  Molien series for a certain group of order 52.
 * https://oeis.org/A005916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3+6*n^2+50*n+126)/312 - n*(n mod 2)/8 + ((n+3)^3
 *            mod 4)/4)
 * Range:     u8 none, u16 n <= 37, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;
    seq_t n3 = n2 * n_seq;
    
    seq_t parity = n_seq & 1;
    
    seq_t r = (n_seq + 3) & 3;
    seq_t cube_mod4 = (r * r * r) & 3;
    
    seq_t numerator = n3 + 6 * n2 + 50 * n_seq + 126 - 39 * n_seq * parity + 78 * cube_mod4;
    
    return numerator / 312;
}
