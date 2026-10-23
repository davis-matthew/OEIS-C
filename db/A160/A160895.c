/*
 * A160895  a(n) = Sum_{d|n} Moebius(n/d)*d^(b-1)/phi(n) for b = 7.
 * https://oeis.org/A160895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = J_6(n)/phi(n) (equivalently a(n) = J_6(n)/phi(n) =
 *            A069091(n)/A000010(n)) For prime power p^e: a(p^e) = p^(5e-5) *
 *            (1 + p + p^2 + p^3 + p^4 + p^5)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 69, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) /* a(1) = 1, and a(0) is undefined but return 0 for safety */
        return (seq_t) (n == 1 ? 1 : 0);

    uint64_t temp = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* Factorise n by trial division (safe for n up to the tested limits) */
    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p != 0)
            continue;

        int e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }

        /* Compute sum = 1 + p + p^2 + p^3 + p^4 + p^5 */
        seq_t sum = (seq_t)1;
        seq_t pow = (seq_t)1;
        for (int i = 1; i <= 5; ++i) {
            pow *= (seq_t)p;
            sum += pow;
        }

        /* Compute p_pow = p^(5*(e-1)) */
        seq_t p_pow = (seq_t)1;
        int exp = 5 * (e - 1);
        for (int i = 0; i < exp; ++i)
            p_pow *= (seq_t)p;

        seq_t term = p_pow * sum;
        result *= term;
    }

    /* Handle the remaining prime factor > sqrt(original n) */
    if (temp > 1) {
        uint64_t p = temp;          /* remaining prime */
        seq_t sum = (seq_t)1;
        seq_t pow = (seq_t)1;
        for (int i = 1; i <= 5; ++i) {
            pow *= (seq_t)p;
            sum += pow;
        }
        seq_t term = sum;           /* exponent e=1, so p_pow = 1 */
        result *= term;
    }

    return result;
}
