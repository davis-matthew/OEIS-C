/*
 * A045771  Number of similar sublattices of index n^2 in root lattice D_4.
 * https://oeis.org/A045771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   multiplicative: a(1) = 1 a(2^p) = 1 a(p^e) (p odd) = (e+1)*p^e +
 *            (2*(1 + (e*(p-1)-1)*p^e))/((p-1)^2)
 * Range:     u8 n <= 12, u16 n <= 1181, u32 n <= 9964, u64 n <= 9964,
 *            u128 n <= 9964, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* ensure n >= 1; a(1) = 1 by definition */
    if (n < 1) return 0;

    seq_t result = 1;          /* cumulative product of local prime powers */
    uint64_t temp   = (uint64_t)n;   /* copy of n for factoring */

    /* handle factors of 2 (always contribute factor 1) */
    while (temp % 2 == 0) {
        temp /= 2;
    }

    /* trial division for odd primes */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }

            /* compute a(p^e) for odd prime p */
            seq_t pow = 1;
            for (int i = 0; i < e; ++i) pow *= (seq_t)p;   /* p^e */

            seq_t term1 = (seq_t)(e + 1) * pow;            /* (e+1)*p^e */

            seq_t coeff = (seq_t)e * (seq_t)(p - 1);       /* e*(p-1) */
            coeff -= 1;                                    /* e*(p-1) - 1 */

            seq_t num   = 1 + coeff * pow;                /* 1 + (e*(p-1)-1)*p^e */
            num  *= 2;                                    /* 2 * that */

            seq_t denom = (seq_t)(p - 1);
            denom *= denom;                               /* (p-1)^2 */

            seq_t div = num / denom;                      /* integer division */

            seq_t pa = term1 + div;                       /* a(p^e) */

            result *= pa;                                 /* multiply into result */
        }
    }

    /* final factor (prime > sqrt(original n)) if any */
    if (temp > 1) {                 /* remaining prime factor to the first power */
        /* temp is odd, so apply the formula with e = 1 */
        uint64_t p = temp;
        int e = 1;
        seq_t pow = (seq_t)p;                       /* p^1 */
        seq_t term1 = (seq_t)(e + 1) * pow;         /* (e+1)*p^e = 2*p */
        seq_t coeff = (seq_t)e * (seq_t)(p - 1);    /* e*(p-1) = p-1 */
        coeff -= 1;                                 /* p-2 */
        seq_t num = 1 + coeff * pow;                /* 1 + (p-2)*p = p^2 - p + 1 */
        num *= 2;                                   /* 2*(p^2 - p + 1) */
        seq_t denom = (seq_t)(p - 1);
        denom *= denom;                             /* (p-1)^2 */
        seq_t div = num / denom;                    /* integer division */
        seq_t pa = term1 + div;                     /* a(p) */
        result *= pa;
    }

    return result;
}
