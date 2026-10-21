/*
 * A055246  At step number k >= 1 the 2^(k-1) open intervals that are erased
 *          from [0,1] in the Cantor middle-third set construction are I(k,n)
 *          = (a(n)/3^k, (1+a(n))/3^k), n=1..2^(k-1).
 * https://oeis.org/A055246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 6 * Σ_{i>=0} bit_i(n-1) * 3^i, where bit_i(n-1) is
 *            the i-th binary bit of (n-1). This is equivalent to a(n) = 1 + 6
 *            * f(n-1) with f(k) interpreting the binary representation of k
 *            as a base‑3 number
 * Range:     u8 n <= 16, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* n >= 1; we compute k = n-1, then sum bits * 3^i */
    idx_t k = n - 1;
    seq_t sum = 0;          /* Σ bit_i * 3^i  */
    seq_t pow3 = 1;         /* current power of 3 (3^i) */

    while (k) {
        if (k & 1U) {
            sum += pow3;
        }
        pow3 *= 3;
        k >>= 1;
    }

    return 1 + 6 * sum;
}
