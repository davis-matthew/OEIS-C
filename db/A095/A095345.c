/*
 * A095345  a(n) is the length of the n-th run in A095346.
 * https://oeis.org/A095345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 if n = 2*ceil(k*φ) for some k ≥ 1, otherwise a(n) = 1
 *            (φ = (1+√5)/2). The test is performed using integer arithmetic
 *            with a high‑precision rational approximation of φ based on the
 *            30th convergent of its continued fraction, namely φ ≈
 *            1346269/832040. This guarantees correct results for al...
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Run indices that are odd cannot be 3‑runs. */
    if ((n & 1ULL) != 0ULL)
        return (seq_t)1;

    /* m = n / 2.  This is the target value of ceil(k*φ). */
    idx_t m = n >> 1U;

    /* Use the convergent φ ≈ 1346269 / 832040.
       FK  = 832040,   FKp1 = 1346269. */
    static const unsigned long long FK  = 832040ULL;      /* denominator of φ */
    static const unsigned long long FKp1 = 1346269ULL;    /* numerator of φ  */

    /* k = floor(m / φ) ≈ floor(m * FK / FKp1). */
    __int128 prod_k = (__int128)m * FK;
    idx_t k = (idx_t)(prod_k / FKp1);

    /* ceil(k*φ) = floor( (k * FKp1 + FK - 1) / FK ). */
    __int128 prod_c = (__int128)k * FKp1 + (FK - 1ULL);
    idx_t ceil_k_phi = (idx_t)(prod_c / FK);

    return (seq_t)(ceil_k_phi == m ? 3 : 1);
}
