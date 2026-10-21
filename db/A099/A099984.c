/*
 * A099984  Bisection of A007947.
 * https://oeis.org/A099984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A007947(2*n-1) = rad(2*n-1) where rad(k) is the product
 *            of the distinct odd prime factors of k. The sequence a(n) is
 *            thus the radical of the odd index 2*n‑1
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* m is the odd index 2*n-1 */
    seq_t m = (seq_t)2 * n - 1;
    seq_t prod = 1;          /* product of distinct primes found */
    seq_t temp = m;          /* remaining part to factor */

    /* m is odd, so we skip the factor 2 */
    for (seq_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            prod *= p;                /* include this prime once */
            while (temp % p == 0) {
                temp /= p;             /* remove all copies */
            }
        }
    }
    /* if anything remains, it is a prime > sqrt(original m) */
    if (temp > 1) {
        prod *= temp;
    }
    return prod;
}
