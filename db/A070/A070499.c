/*
 * A070499  a(n) = n^3 mod 37.
 * https://oeis.org/A070499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 37
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A070499: the values of n raised to the third power 
   reduced modulo 37. */
seq_t a(idx_t n) {
    /* Bring n into the range [0,36] by taking modulo 37. */
    seq_t m = (seq_t)(n % 37);
    /* Compute m^3 modulo 37 using repeated modular multiplication. */
    seq_t res = (m * m) % 37;  /* m^2 mod 37 */
    res = (res * m) % 37;      /* m^3 mod 37 */
    return res;
}
