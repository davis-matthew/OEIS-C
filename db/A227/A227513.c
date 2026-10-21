/*
 * A227513  Round(-1/n + 1/log((2n+1)/(2n-1))).
 * https://oeis.org/A227513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 710, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A227513
 * Formula: a(n) = 12*n^3 - round(9*n/5)
 * where round(x) is the nearest integer (ties rounded up).
 * The function returns a(n) for all n >= 1 using integer arithmetic only.
 */
seq_t a(idx_t n)
{
    seq_t n_big = (seq_t)n;          /* cast index to the sequence type */
    seq_t n2 = n_big * n_big;        /* n^2 */
    seq_t n3 = n2 * n_big;           /* n^3 */
    seq_t round9n = (9 * n_big + 2) / 5;  /* round(9*n/5) : floor((9*n+2)/5) */
    return 12 * n3 - round9n;        /* 12*n^3 - round(9*n/5) */
}
