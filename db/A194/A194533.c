/*
 * A194533  Jordan function ratio J_8(n)/J_2(n).
 * https://oeis.org/A194533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e||n} p^(6*(e-1)) * (p^2+1) * (p^4+1) (n≥1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 39, u64 n <= 1517,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast exponentiation for any seq_t type (int or big integer) */
static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* contribution of a prime factor p^e to the product */
static seq_t factor_for_prime(idx_t p, idx_t e) {
    seq_t term = pow_seq((seq_t)p, 6 * (e - 1));
    seq_t pp  = pow_seq((seq_t)p, 2);
    seq_t pp4 = pow_seq((seq_t)p, 4);
    term *= (pp + 1);
    term *= (pp4 + 1);
    return term;
}

seq_t a(idx_t n) {
    if (n == 0)  /* undefined but return 0 to avoid UB */
        return 0;

    seq_t result = 1;
    idx_t m = n;
    idx_t e = 0;

    /* factor 2 */
    while ((m & 1ULL) == 0) {
        ++e;
        m >>= 1;
    }
    if (e > 0)
        result *= factor_for_prime(2, e);

    /* odd prime factors */
    for (idx_t p = 3; p <= m; p += 2) {
        if (p > m / p) /* p^2 > m → stop */
            break;
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                ++e;
                m /= p;
            }
            result *= factor_for_prime(p, e);
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (m > 1)
        result *= factor_for_prime(m, 1);

    return result;
}
