/*
 * A143869  An integer k is called regular (mod n) if there is an integer x
 *          such that k^2 x == k (mod n). Then these numbers are the sum of
 *          regular integers k (mod n) such that 1 <= k <= n for n=1,2,... .
 * https://oeis.org/A143869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(A055653(n)+1)/2
 * Range:     u8 n <= 22, u16 n <= 361, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1, but guard */

    idx_t tmp = n;                    /* remaining part to factor */
    seq_t prod = 1;                   /* product of (1 + φ(p^e)) over all prime powers */

    /* factor the 2‑part separately to avoid even increments in the main loop */
    if ((tmp & 1) == 0) {
        idx_t pow = 1;                /* current power of 2, eventually 2^e   */
        while ((tmp & 1) == 0) {      /* while 2 divides tmp */
            tmp >>= 1;                /* divide tmp by 2                     */
            pow <<= 1;                /* multiply pow by 2                   */
        }
        /* term = 1 + φ(2^e) = 1 + 2^(e-1) = 1 + pow/2 */
        seq_t term = (seq_t)1 + (seq_t)(pow / 2);
        prod *= term;
    }

    /* loop over odd primes */
    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            idx_t pow = 1;
            while (tmp % p == 0) {
                tmp /= p;
                pow *= p;              /* pow = p^e                                */
            }
            /* term = 1 + φ(p^e) = 1 + p^(e-1)*(p-1) = 1 + (pow/p)*(p-1) */
            seq_t term = (seq_t)1 + (seq_t)(pow / p) * (seq_t)(p - 1);
            prod *= term;
        }
    }

    /* any remaining prime factor is > sqrt(tmp) and occurs to the first power */
    if (tmp > 1) {
        /* term = 1 + φ(tmp) = 1 + (tmp-1) = tmp  */
        seq_t term = (seq_t)1 + (seq_t)(tmp - 1);
        prod *= term;
    }

    /* a(n) = n * (prod + 1) / 2  where prod = A055653(n) */
    seq_t result = (seq_t)n * (prod + 1) / 2;
    return result;
}
