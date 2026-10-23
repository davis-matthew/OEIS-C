/*
 * A033443  Number of edges in 11-partite Turán graph of order n.
 * https://oeis.org/A033443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} floor(10*i/11)
 * Range:     u8 n <= 7, u16 n <= 175, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q = n / 11;          /* number of complete 11‑blocks */
    idx_t r = n % 11;          /* remaining part (0..10) */

    seq_t n_seq = (seq_t)n;
    seq_t n_n1   = n_seq * (n_seq + 1);          /* n*(n+1)  */

    /* 5*n*(n+1) part of numerator */
    seq_t term = (seq_t)5 * n_n1;

    /* sum of remainders r_i over 1..n : 55*q + (r*11 - r*(r+1)/2) */
    seq_t sum_r = (seq_t)55 * (seq_t)q;

    seq_t r_seq = (seq_t)r;
    seq_t partial = r_seq * 11 - (r_seq * (r_seq + 1)) / 2;
    sum_r += partial;

    seq_t numer   = term - sum_r;   /* numerator of (5*n*(n+1) - sum_r) */
    seq_t result  = numer / (seq_t)11;

    return result;
}
