/*
 * A168209  a(n) = 5*n - a(n-1) + 1 with n>1, a(1)=3.
 * https://oeis.org/A168209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 7 + 5*(-1)^n)/4 ; a(1)=3
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* index offset is 1; behavior for n==0 is undefined for this sequence */
    seq_t n_seq = (seq_t)n;
    seq_t term = n_seq * (seq_t)10u + (seq_t)7u;
    /* add 5 for even n, subtract 5 for odd n */
    if ((n & 1) == 0) {
        term += (seq_t)5u;
    } else {
        term -= (seq_t)5u;
    }
    return term / (seq_t)4u;
}
