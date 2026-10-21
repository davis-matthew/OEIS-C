/*
 * A081601  Numbers m such that 3 does not divide Sum_{k=0..m} binomial(2k,k)
 *          = A006134(m).
 * https://oeis.org/A081601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3 * interpret_binary_as_base3(n-1) where
 *            interpret_binary_as_base3(k) is the integer formed by writing k
 *            in binary and reading those digits as base‑3 digits
 * Range:     u8 n <= 20, u16 n <= 640, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t k = n - 1;        /* n≥1, so k is well defined */
    seq_t m = 0;            /* result of the base‑3 number */
    seq_t pow3 = 1;         /* current power of 3 (3^i) */

    while (k) {
        if (k & 1)          /* add 3^i if the i‑th binary bit is set */
            m += pow3;
        pow3 *= 3;          /* advance to next power of 3 */
        k >>= 1;
    }
    m *= 3;                 /* final multiplication by 3 */
    return m;
}
