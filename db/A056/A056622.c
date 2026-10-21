/*
 * A056622  a(n) = A000188(n)/A055229(n).
 * https://oeis.org/A056622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product of p^e where the factor p^e of n contributes p^(
 *            e/2 ) if e even, 1 if e==1, or p^((e-3)/2) if e odd and e>1
 * Range:     u8 n <= 9048, u16 n <= 9048, u32 n <= 9048, u64 n <= 9048,
 *            u128 n <= 9048, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)        /* the sequence is defined for n ≥ 1, but guard anyway */
        return 1;      /* a(1) = 1 */

    seq_t result = 1;
    uint64_t tmp = (uint64_t)n;   /* n fits in uint64_t by definition */

    /* factor 2 separately */
    uint64_t e = 0;
    while ((tmp & 1ULL) == 0) {    /* while tmp is divisible by 2 */
        e++;
        tmp >>= 1;                 /* tmp /= 2; use shift for speed */
    }
    if (e > 0) {
        uint64_t exp;
        if ((e & 1ULL) == 0)          /* e even */
            exp = e >> 1;
        else if (e == 1)              /* e == 1 */
            exp = 0;
        else                          /* e odd > 1 */
            exp = (e - 3ULL) >> 1;
        for (uint64_t i = 0; i < exp; ++i)
            result *= (seq_t)2;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                e++;
                tmp /= p;
            }
            uint64_t exp;
            if ((e & 1ULL) == 0)          /* e even */
                exp = e >> 1;
            else if (e == 1)              /* e == 1 */
                exp = 0;
            else                          /* e odd > 1 */
                exp = (e - 3ULL) >> 1;
            for (uint64_t i = 0; i < exp; ++i)
                result *= (seq_t)p;
        }
    }
    /* if any prime factor > sqrt(tmp) remains, it must be a single prime */
    if (tmp > 1) {
        uint64_t p = tmp;
        uint64_t e = 1;   /* its exponent is 1 */
        uint64_t exp = 0; /* e==1 => factor 1 */
        /* No multiplication needed */
        (void)p; (void)exp; /* silence unused variable warnings */
    }

    return result;
}
