/*
 * A069531  Smallest positive k such that 10^k + 1 is divisible by n, or 0 if
 *          no such number exists.
 * https://oeis.org/A069531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k>0 such that 10^k + 1 is divisible by n, * or 0
 *            if no such k exists
 * Range:     u8 n <= 540, u16 n <= 8617, u32 n <= 8617, u64 n <= 8617,
 *            u128 n <= 8617, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute Euler's totient for n */
static idx_t phi_of(idx_t m) {
    idx_t result = m;
    idx_t p = 2;
    while (p * p <= m) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            result -= result / p;
        }
        ++p;
    }
    if (m > 1) result -= result / m;
    return result;
}

seq_t a(idx_t n) {
    /* n must be >=1 according to OEIS offset; if 0, return 0 */
    if (n == 0) return 0;

    /* If n shares a factor 2 or 5 with 10, 10^k+1 is never divisible by n */
    if (n % 2 == 0 || n % 5 == 0) return 0;

    idx_t phi = phi_of(n);
    seq_t pow_mod = 1 % n;   /* current value of 10^k modulo n */

    for (idx_t k = 1; k <= phi; ++k) {
        pow_mod = (pow_mod * 10) % n;
        /* check if 10^k + 1 ≡ 0 (mod n) */
        if ((pow_mod + 1) % n == 0) return (seq_t)k;
    }
    return 0;   /* no such k exists */
}
