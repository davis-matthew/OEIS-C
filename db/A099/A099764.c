/*
 * A099764  a(n) = n^2 * (n+1)^2 * (n+2)^2 = 36*A001249(n-1).
 * https://oeis.org/A099764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2 * (n+1)^2 * (n+2)^2 (offset 0) Computes the n‑th term
 *            of OEIS A099764. Uses a 128‑bit intermediate to avoid overflow
 *            for 64/128 bit builds, and works for any arbitrary‑precision
 *            seq_t as long as it supports casting from __int128
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 39, u64 n <= 1624,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case n = 0 gives 0 without overflow */
    if (n == 0)
        return 0;

    /* Convert operands to seq_t first, then to 128‑bit for multiplication */
    seq_t n_seq   = (seq_t)n;
    seq_t n1_seq  = (seq_t)(n + 1);
    seq_t n2_seq  = (seq_t)(n + 2);

    /* Compute squares as 128‑bit, then cast back to seq_t */
    __int128 sq_n  = (__int128)n_seq * (__int128)n_seq;
    __int128 sq_n1 = (__int128)n1_seq * (__int128)n1_seq;
    __int128 sq_n2 = (__int128)n2_seq * (__int128)n2_seq;

    /* Final product */
    __int128 prod = sq_n * sq_n1 * sq_n2;

    return (seq_t)prod;   /* cast back to result type */
}
