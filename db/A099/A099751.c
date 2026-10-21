/*
 * A099751  Number of ways to write n as differences of (-4)-gonal numbers. If
 *          pe(n):=1/2*n*((e-2)*n+(4-e)) is the n-th e-gonal number, then a(n)
 *          = |{(m,k) of Z X Z; pe(-1)(m+k)-pe(m-1)=n}| for e=-4.
 * https://oeis.org/A099751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, easy, nonn
 * Formula:   a(n) = ∏ over prime powers p^e || n of a(p^e) = e-1 if p = 2 (e
 *            > 0) (1 if e=0) 1 if p = 3 e+1 if p is an odd prime > 3 The
 *            function is multiplicative, thus a(n) is the product of the
 *            contributions of all prime powers in n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* not used; a(0) undefined in OEIS */ return 0;

    seq_t res = 1;
    idx_t temp = n;

    /* Handle power of 2 */
    idx_t e2 = 0;
    while ((temp & 1ULL) == 0) {
        e2++;
        temp >>= 1;
    }
    if (e2 > 0) {
        res *= (seq_t)(e2 - 1);
    }

    /* Handle remaining odd primes */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t ep = 0;
            while (temp % p == 0) {
                ep++;
                temp /= p;
            }
            if (p == 3) {
                /* factor 1 – do nothing */
            } else {
                res *= (seq_t)(ep + 1);
            }
        }
    }

    /* If a prime factor remains */
    if (temp > 1) {
        if (temp == 3) {
            /* factor 1 – do nothing */
        } else {
            res *= (seq_t)2;     /* exponent = 1 → e+1 = 2 */
        }
    }

    return res;
}
