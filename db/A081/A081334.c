/*
 * A081334  a(n) = sigma(2*n^2) modulo 4.
 * https://oeis.org/A081334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(2*n*n) modulo 4
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts the odd exponents of primes ≡ 1 (mod 4) in n.
   Returns 3 if the count is even, 1 if odd. */
seq_t a(idx_t n)
{
    /* we need only odd part of n; even part ignored for counting  */
    uint64_t m = (uint64_t)n;
    while ((m & 1) == 0)                /* remove powers of two          */
        m >>= 1;

    int flag = 0;   /* 0 => even count of (p ≡ 1 mod 4, exp odd) */
    uint64_t temp = m, p = 3;

    for (; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            if ((p % 4 == 1) && (exp & 1))
                flag ^= 1;               /* toggle parity for odd exponent */
        }
    }
    if (temp > 1) {                       /* remaining prime factor */
        if ((temp % 4 == 1) && 1)          /* exponent is 1 (odd)          */
            flag ^= 1;
    }

    return flag ? (seq_t)1 : (seq_t)3;
}
