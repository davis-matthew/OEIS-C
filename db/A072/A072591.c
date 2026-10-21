/*
 * A072591  In prime factorization of n replace multiplication with bitwise
 *          logical 'and'.
 * https://oeis.org/A072591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = bitwise AND of the distinct prime factors of n; if n is
 *            even and contains a prime ≡ 1 (mod 4), then a(n) = 0
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Special case */
    if (n == 1) return 1;

    int even = (n & 1) == 0;          /* N is even? */
    int has1mod4 = 0;                 /* Any prime factor ≡ 1 (mod 4)? */
    idx_t m = n;                      /* Working copy of n */

    int first = 1;                    /* No prime processed yet */
    seq_t res = 0;                    /* Result of successive ANDs */

    /* Factor out 2 */
    if ((m & 1) == 0) {
        if (first) { res = (seq_t)2; first = 0; }
        else       { res &= (seq_t)2; }
        while ((m & 1) == 0) m /= 2;
    }

    /* Factor odd primes up to sqrt(m) */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            if (first) { res = (seq_t)p; first = 0; }
            else       { res &= (seq_t)p; }
            if (even && (p % 4) == 1) has1mod4 = 1;
            while (m % p == 0) m /= p;
        }
    }

    /* Remaining prime factor > sqrt(original n) */
    if (m > 1) {
        if (first) { res = (seq_t)m; first = 0; }
        else       { res &= (seq_t)m; }
        if (even && (m % 4) == 1) has1mod4 = 1;
    }

    /* Apply the special zero rule */
    if (even && has1mod4) return 0;
    return res;
}
