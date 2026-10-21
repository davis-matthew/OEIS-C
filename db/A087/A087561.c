/*
 * A087561  Number of solutions to x^2 + 2y^2 == 0 (mod n).
 * https://oeis.org/A087561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   multiplicative a(1) = 1 a(2^e) = 2^e for odd prime p if p % 8 ==
 *            5 or 7 : a(p^e) = p^(2*floor(e/2)) else : a(p^e) = ((p-1)*e + p)
 *            * p^(e-1)
 * Range:     u8 n <= 80, u16 n <= 7352, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t r = 1;
    while (exp) {
        if (exp & 1) r *= base;
        base *= base;
        exp >>= 1;
    }
    return r;
}

seq_t a(idx_t n) {
    if (n <= 1) return 1;   /* a(1)=1 ; a(0) unused but return 1 */

    seq_t result = 1;
    idx_t m = n;

    /* handle powers of 2 */
    idx_t e2 = 0;
    while ((m & 1) == 0) {
        m >>= 1;
        e2++;
    }
    if (e2) result *= pow_seq(2, e2);

    /* factor odd part */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            seq_t term;
            if (p % 8 == 5 || p % 8 == 7) {
                /* exponent = 2*floor(e/2) == e if e even, else e-1 */
                idx_t exp = e - (e & 1);   /* remove 1 if e is odd */
                term = pow_seq((seq_t)p, exp);
            } else {
                /* ((p-1)*e + p) * p^(e-1) */
                seq_t p_pow = (e > 1) ? pow_seq((seq_t)p, e - 1) : 1;
                term = ((seq_t)(p - 1) * (seq_t)e + (seq_t)p) * p_pow;
            }
            result *= term;
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (m > 1) {
        idx_t p = m;
        idx_t e = 1;
        seq_t term;
        if (p % 8 == 5 || p % 8 == 7) {
            term = 1;
        } else {
            term = ((seq_t)(p - 1) * (seq_t)e + (seq_t)p);   /* p^(e-1)=1 */
        }
        result *= term;
    }

    return result;
}
