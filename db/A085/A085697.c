/*
 * A085697  a(n) = T(n)^2, where T(n) = A000073(n) is the n-th tribonacci
 *          number.
 * https://oeis.org/A085697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 39, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = (T(n))^2
 * Where T(n) is the Tribonacci sequence:
 *   T(0) = 0, T(1) = 0, T(2) = 1,
 *   T(k) = T(k-1) + T(k-2) + T(k-3)  for k ≥ 3
 * The function returns the square of T(n) using only integer
 * arithmetic available to seq_t.  It works for all values of n
 * that fit into the provided idx_t, and produces exact results
 * when seq_t can accommodate them.
 */
seq_t a(idx_t n) {
    /* Handle the small indices directly */
    if (n == 0 || n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* Compute Tribonacci numbers iteratively */
    seq_t t0 = (seq_t)0;   /* T(0) */
    seq_t t1 = (seq_t)0;   /* T(1) */
    seq_t t2 = (seq_t)1;   /* T(2) */
    seq_t t3;

    for (idx_t i = 3; i <= n; ++i) {
        t3 = t2 + t1 + t0; /* T(i) */
        t0 = t1;
        t1 = t2;
        t2 = t3;
    }

    /* Return the square of the nth Tribonacci number */
    return t2 * t2;
}
