/*
 * A234306  a(n) = n + 1 - d(2n), where d(n) is the number of divisors of n.
 * https://oeis.org/A234306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 1 - d(2n)
 * Range:     u8 n <= 261, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset safety */

    idx_t temp = n;
    int e = 0;
    while ((temp & 1ULL) == 0) {   /* count factor 2 in n */
        e++;
        temp >>= 1;
    }

    seq_t d = (seq_t)(e + 2); /* exponent of 2 in 2n is e+1 -> (e+1)+1 */

    for (idx_t p = 3; p <= temp / p; p += 2) {
        int cnt = 0;
        while (temp % p == 0) {
            cnt++;
            temp /= p;
        }
        if (cnt) d *= (seq_t)(cnt + 1);
    }
    if (temp > 1) d *= 2;       /* remaining prime factor */

    seq_t res = (seq_t)n + 1 - d;
    return res;
}
