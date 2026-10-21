/*
 * A090624  If n = Product(pj^ej), i.e., written in its prime factorization,
 *          then a(n) = max_j{(pj-1)*ej}.
 * https://oeis.org/A090624
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_j (p_j-1) * e_j for n = Π p_j^e_j (prime
 *            factorisation)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute the sequence value for any n >= 2.
   For n < 2 the behaviour is undefined; return 0. */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    uint64_t m = (uint64_t)n;
    seq_t max_val = 0;

    /* Handle factor 2 separately */
    if ((m & 1) == 0) {
        uint64_t e = 0;
        while ((m & 1) == 0) {
            m >>= 1;          /* m /= 2 */
            ++e;
        }
        seq_t val = ((seq_t)2 - 1) * (seq_t)e;  /* (2-1)*e = e */
        if (val > max_val)
            max_val = val;
    }

    /* Trial division by odd primes up to sqrt(m) */
    for (uint64_t p = 3; (__int128)p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t val = ((seq_t)p - 1) * (seq_t)e;
            if (val > max_val)
                max_val = val;
        }
    }

    /* Remaining prime factor (if any) */
    if (m > 1) {
        seq_t val = ((seq_t)m) - 1;  /* exponent is 1 */
        if (val > max_val)
            max_val = val;
    }

    return max_val;
}
