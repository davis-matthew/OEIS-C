/*
 * A088836  a(n) = (A020639(2n+1) + A006530(2n+1))/2, arithmetic mean of max
 *          and min prime factor applied to odd numbers (i.e., when this mean
 *          is an integer).
 * https://oeis.org/A088836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (spf(2n+1)+lpf(2n+1))/2 (arithmetic mean of min and max
 *            prime factor of 2n+1)
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* m = 2n+1 is the odd number to factor */
    idx_t m = n * 2 + 1;
    idx_t temp = m;

    seq_t spf = 0;   /* smallest prime factor */
    seq_t lpf = 0;   /* largest  prime factor */

    /* factor by trial division – sufficient for the expected range (n up to ~10⁴) */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            if (spf == 0) spf = (seq_t)p;   /* first factor found */
            lpf = (seq_t)p;                  /* keep the last (largest) factor seen so far */
            while (temp % p == 0)
                temp /= p;
        }
    }

    /* if anything remains, it is a prime factor greater than sqrt(original) */
    if (temp > 1) {
        if (spf == 0) spf = (seq_t)temp;
        lpf = (seq_t)temp;
    }

    /* for a prime m both spf and lpf should be m */
    if (spf == 0) {          /* this happens only when m == 1 (not in our domain) */
        spf = lpf = (seq_t)m;
    }

    /* arithmetic mean – guaranteed to be integer because spf and lpf have the same parity for odd numbers */
    return (spf + lpf) / 2;
}
