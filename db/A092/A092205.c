/*
 * A092205  Number of units in the imaginary quadratic field Q(sqrt(-n)).
 * https://oeis.org/A092205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 if the squarefree part of n equals 1; 6 if it equals 3;
 *            otherwise 2. (Number of units in Q(√‑n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t tmp = n;
    seq_t sf = 1;          /* squarefree part */
    idx_t p = 2;

    while (p * p <= tmp) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            if (e & 1)      /* odd exponent → keep one copy */
                sf *= p;
        }
        /* skip even numbers after 2 */
        if (p == 2) p = 3;
        else p += 2;
    }
    if (tmp > 1)            /* remaining prime factor with exponent 1 */
        sf *= tmp;

    if (sf == 1) return (seq_t)4;
    if (sf == 3) return (seq_t)6;
    return (seq_t)2;
}
