/*
 * A062956  a(n) = h(n^2) - h(n), where h(n) is the half-totient function
 *          (A023022).
 * https://oeis.org/A062956
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*h(n) where h(n) = φ(n)/2 for n > 2 * = (n-1)*φ(n)/2
 *            * (A062956) * Compute φ(n) by trial division (fast enough for n
 *            up to ~10^6)
 * Range:     u8 n <= 28, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t phi_of(idx_t n)
{
    seq_t result = (seq_t)n;          /* φ(n) starts as n */
    idx_t m = n;                      /* copy for division */

    for (idx_t p = 2; (seq_t)p * p <= (seq_t)m; ++p)
    {
        if (m % p == 0)
        {
            while (m % p == 0)
                m /= p;
            result = result / (seq_t)p * ((seq_t)p - 1);
        }
    }

    if (m > 1)                       /* m itself is a prime factor > sqrt(n) */
        result = result / (seq_t)m * ((seq_t)m - 1);

    return result;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    seq_t ph = phi_of(n);
    /* a(n) = (n-1) * φ(n) / 2 */
    return ((seq_t)(n - 1) * ph) / 2;
}
