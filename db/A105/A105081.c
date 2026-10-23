/*
 * A105081  a(n) = 1 + A003188(n - 1), n >= 1.
 * https://oeis.org/A105081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = 1 + gray(n-1), where gray(k) = k XOR (k >> 1)
 * Range:     u8 n <= 170, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute the n-th term (n ≥ 1) of OEIS A105081.
    The computation uses only integer arithmetic and bitwise operations,
    guaranteed to be O(1). */
seq_t a(idx_t n)
{
    /* n is ≥ 1, so n-1 is safely computed in idx_t (uint64_t). */
    idx_t k = n - 1;                   /* k = n - 1 */
    idx_t gray = k ^ (k >> 1);          /* Gray code of k */
    return (seq_t)gray + 1;            /* a(n) = 1 + gray(k) */
}
