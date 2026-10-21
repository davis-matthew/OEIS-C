/*
 * A086665  Difference between sum of divisors of n and integer log of n,
 *          i.e., A000203(n) - A001414(n).
 * https://oeis.org/A086665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = σ(n) - A001414(n)
 * Range:     u8 n <= 107, u16 n <= 9912, u32 n <= 9912, u64 n <= 9912,
 *            u128 n <= 9912, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  sigma(n) is the sum of all divisors of n, */
/*  A001414(n) is the sum of the prime factors of n counted with multiplicity. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;   /* undefined for n = 0, harmless for callers */
    }

    seq_t sigma  = (seq_t)1;   /* σ(1) = 1 */
    seq_t intlog = (seq_t)0;   /* sum of prime factors (with multiplicity) */

    uint64_t m = (uint64_t)n;  /* 64‑bit copy for factorisation */

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            seq_t term   = (seq_t)1;   /* running sum for this prime */
            seq_t power  = (seq_t)1;   /* current power of p */

            while (m % p == 0) {
                m /= p;
                intlog += (seq_t)p;   /* add prime for each occurrence */
                power  *= (seq_t)p;
                term   += power;       /* add next power to divisor sum */
            }
            sigma *= term;             /* multiply σ by σ(p^a) */
        }
    }

    /* Remaining prime factor > sqrt(n) */
    if (m > 1) {
        intlog += (seq_t)m;
        sigma *= ((seq_t)1 + (seq_t)m);
    }

    return sigma - intlog;
}
