/*
 * A054429  Simple self-inverse permutation of natural numbers: list each
 *          block of 2^n numbers (from 2^n to 2^(n+1) - 1) in reverse order.
 * https://oeis.org/A054429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = 2^⌊log₂n⌋ + 2^⊤log₂n−1 − (n − 2^⌊log₂n⌋) (i.e. reverse
 *            each block of size 2^k numbers)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is ≥ 1 (offset 1); for n==0 the result is arbitrary */
    if (n == 0) return (seq_t)0;

    /* highest power of two ≤ n */
    uint64_t k = 63 - __builtin_clzll(n);   /* floor(log2(n)) */

    seq_t start = ((seq_t)1) << k;          /* 2^k  */
    seq_t last  = (start << 1) - ((seq_t)1);/* 2^(k+1) – 1 */

    /* diff = n – start, computed in seq_t to avoid type clashes */
    seq_t diff  = ((seq_t)n) - start;

    return last - diff;
}
