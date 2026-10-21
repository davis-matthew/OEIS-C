/*
 * A056134  Smallest positive integer which is the geometric mean of n and an
 *          integer other than n.
 * https://oeis.org/A056134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest integer a such that a^2 is a multiple of n and a
 *            ≠ n. If n is square‑free then a(n)=2*n; otherwise
 *            a(n)=∏p_i^{ceil(e_i/2)} where n=∏p_i^{e_i}
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safety, not used for the offset 1 */
    seq_t res = 1;
    bool squarefree = true;
    unsigned long long m = (unsigned long long)n;  /* temporary copy */

    /* handle factor 2 separately */
    int e = 0;
    while (m % 2 == 0) {
        m /= 2;
        e++;
    }
    if (e) {
        if (e >= 2) squarefree = false;
        int exp = (e + 1) / 2;
        for (int i = 0; i < exp; ++i) res *= (seq_t)2;
    }

    /* odd prime factors */
    for (unsigned long long p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            if (e >= 2) squarefree = false;
            int exp = (e + 1) / 2;
            for (int i = 0; i < exp; ++i) res *= (seq_t)p;
        }
    }

    /* remaining prime factor (if any) */
    if (m > 1) {
        /* exponent e = 1, so exp = (1+1)/2 = 1 */
        res *= (seq_t)m;
    }

    /* if n is square‑free (all exponents 1), multiply by 2 */
    if (squarefree) res *= (seq_t)2;

    return res;
}
