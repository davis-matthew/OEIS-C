/*
 * A145388  Sum of (k,n)_* for k=1,2,...,n, where (k,n)_* is the greatest
 *          divisor of k which is a unitary divisor of n.
 * https://oeis.org/A145388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = ∏_{p^e‖n} (2·p^e − 1). The function is multiplicative
 *            with a(1)=1. (Product over all prime powers dividing n.) This
 *            implementation factors n by trial division and accumulates the
 *            product using only integer arithmetic on the provided seq_t type
 * Range:     u8 n <= 59, u16 n <= 4289, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* for safety, though n≥1 is assumed */
    seq_t result = (seq_t)1;
    idx_t temp = n;
    /* factor 2 */
    idx_t e = 0;
    while ((temp & (idx_t)1) == 0) {
        temp >>= 1;
        ++e;
    }
    if (e > 0) {
        seq_t p_pow = (seq_t)1;
        for (idx_t i = 0; i < e; ++i) p_pow *= (seq_t)2;
        seq_t term = (seq_t)2 * p_pow - (seq_t)1;
        result *= term;
    }
    /* factor odd primes */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            seq_t p_pow = (seq_t)1;
            for (idx_t i = 0; i < exp; ++i) p_pow *= (seq_t)p;
            seq_t term = (seq_t)2 * p_pow - (seq_t)1;
            result *= term;
        }
    }
    /* remaining prime factor, if any */
    if (temp > 1) {
        seq_t p_pow = (seq_t)temp;          /* exponent is 1 */
        seq_t term = (seq_t)2 * p_pow - (seq_t)1;
        result *= term;
    }
    return result;
}
