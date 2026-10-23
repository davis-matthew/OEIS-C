/*
 * A167352  Totally multiplicative sequence with a(p) = (p+1)*(p-3) = p^2-2p-3
 *          for prime p.
 * https://oeis.org/A167352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = ∏_{p^e‖n} ((p+1)*(p-3))^e; a(3k)=0 for k≥1
 * Range:     i8 n <= 12, i16 n <= 190, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiply |base| by itself exponent times (exponent ≤ 64 for n≤10000) */
static inline seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp > 0) {
        res *= base;
        exp--;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 1)            /* n=1 → a(1)=1 */
        return 1;

    if ((n % 3) == 0)      /* any multiple of 3 gives 0 */
        return 0;

    seq_t result = 1;
    idx_t m = n;
    idx_t f = 2;

    /* trial division up to sqrt(m) */
    while (f * f <= m) {
        if (m % f == 0) {
            idx_t exp = 0;
            while (m % f == 0) {
                m /= f;
                exp++;
            }
            /* use signed arithmetic for the (p+1)(p-3) product */
            int64_t pf = (int64_t)f;
            seq_t part = (seq_t)(pf + 1) * (seq_t)(pf - 3);
            result *= pow_seq_t(part, exp);
        }
        f = (f == 2) ? 3 : f + 2;   /* iterate over 2,3,5,7,... */
    }

    /* remaining factor (prime > sqrt(n)) */
    if (m > 1) {
        int64_t pf = (int64_t)m;
        seq_t part = (seq_t)(pf + 1) * (seq_t)(pf - 3);
        result *= part;             /* exponent is 1 */
    }

    return result;
}
