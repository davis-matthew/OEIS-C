/*
 * A127612  a(n) = denominator of the continued fraction which has the
 *          positive divisors of n as its terms. The terms are written in
 *          order from 1 for the integer part, to n for the final term of the
 *          continued fraction.
 * https://oeis.org/A127612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = denominator of the continued fraction of the positive
 *            divisors of n in increasing order
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* The sequence starts at n = 1.  n = 0 is unused but safe. */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* ------------------------------------------------------------------
     *  Factor n into prime powers.
     *  For n ≤ 10 000 a simple trial division is fast enough.
     * ------------------------------------------------------------------ */
    idx_t m = n;
    idx_t primes[64];
    idx_t exps[64];
    size_t pcnt = 0;

    if ((m & 1ULL) == 0) {
        idx_t e = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            ++e;
        }
        primes[pcnt] = 2;
        exps[pcnt]   = e;
        ++pcnt;
    }
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt]   = e;
            ++pcnt;
        }
    }
    if (m > 1) {
        primes[pcnt] = m;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* ------------------------------------------------------------------
     *  Enumerate all divisors of n from the prime factorisation.
     *  Start with the single divisor 1 and extend multiplicatively.
     * ------------------------------------------------------------------ */
    idx_t divs[512];
    size_t dcnt = 1;
    divs[0] = 1;

    for (size_t fi = 0; fi < pcnt; ++fi) {
        idx_t p = primes[fi];
        idx_t e = exps[fi];
        size_t curcnt = dcnt;
        for (size_t j = 0; j < curcnt; ++j) {
            idx_t base  = divs[j];
            idx_t power = base * p;          /* base * p^1 */
            for (idx_t k = 1; k <= e; ++k) {
                divs[dcnt++] = power;
                power *= p;                  /* next power */
            }
        }
    }

    /* ------------------------------------------------------------------
     *  Sort the divisor list in ascending order.
     * ------------------------------------------------------------------ */
    for (size_t i = 1; i < dcnt; ++i) {
        idx_t key = divs[i];
        size_t j = i;
        while (j > 0 && divs[j - 1] > key) {
            divs[j] = divs[j - 1];
            --j;
        }
        divs[j] = key;
    }

    /* ------------------------------------------------------------------
     *  Compute the denominator of the continued fraction.
     *  Recurrence: q_-2 = 1, q_-1 = 0, q_i = a_i*q_{i-1} + q_{i-2}
     * ------------------------------------------------------------------ */
    seq_t q_prev2 = 1;   /* q_-2 */
    seq_t q_prev1 = 0;   /* q_-1 */
    seq_t q_cur   = 0;

    for (size_t i = 0; i < dcnt; ++i) {
        seq_t ai   = (seq_t)divs[i];
        q_cur      = ai * q_prev1 + q_prev2;
        q_prev2    = q_prev1;
        q_prev1    = q_cur;
    }
    return q_cur;
}
