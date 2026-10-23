/*
 * A187249  Number of cycles with at most 2 alternating runs in all
 *          permutations of [n] (it is assumed that the smallest element of
 *          the cycle is in the first position).
 * https://oeis.org/A187249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1, for n >= 2: a(n) = n * a(n-1) + 2^(n-2)
 *            (because 2^(n-2) is integer for n>=2)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t a_prev = 1;    /* a(1) */
    seq_t pow2  = 1;     /* 2^(2-2) for n=2 */

    for (idx_t k = 2; k <= n; ++k) {
        seq_t term   = ((seq_t)k) * a_prev;   /* k * a_{k-1} */
        seq_t a_next = term + pow2;           /* a_k = k*a_{k-1} + 2^{k-2} */
        pow2 = pow2 * ((seq_t)2);             /* prepare 2^{k-1 - 2} for next iteration */
        a_prev = a_next;
    }

    return a_prev;
}
