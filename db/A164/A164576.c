/*
 * A164576  Integer averages of the set of the first positive squares up to
 *          some n^2.
 * https://oeis.org/A164576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/4*(12*n^2 - 6*n + (-1)^n*(4*n-1) + 1)
 * Range:     u8 n <= 9, u16 n <= 147, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent:  a(n) = ((k+1)*(2*k+1))/6  where k = 3*n-2 (n odd) or 3*n-1 (n even) */

seq_t a(idx_t n) {
    /* n is 1-based index in the sequence A164576 */
    if (n == 0) return 0;  /* value for n<1 is irrelevant for the problem */

    seq_t k;
    if (n & 1)
        k = (seq_t)(3 * n - 2);   /* n odd → k ≡ 1 (mod 6) */
    else
        k = (seq_t)(3 * n - 1);   /* n even → k ≡ 5 (mod 6) */

    /* Compute the integer average of the first k squares:
       S(k) = k(k+1)(2k+1)/6, division by k gives (k+1)(2k+1)/6        */
    return ((k + 1) * ( (seq_t)2 * k + 1 )) / 6;
}
