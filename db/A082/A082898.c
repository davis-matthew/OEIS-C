/*
 * A082898  a(n) = A082895(n)/n, where A082895(n) is the closest number to
 *          sigma(n) which is divisible by n.
 * https://oeis.org/A082898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( ( floor(n/2) + σ(n) ) / n ) where σ(n) is the sum
 *            of all positive divisors of n (A000203)
 * Range:     u8 n <= 83, u16 n <= 99999, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 116 terms through n = 9451 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Compute σ(n) by iterating over the divisors up to √n. */
static seq_t
sigma_direct(idx_t n)
{
    seq_t sum = (seq_t)0;
    uint64_t nn = (uint64_t)n;
    for (uint64_t d = 1; d * d <= nn; ++d)
    {
        if (nn % d == 0)
        {
            sum += (seq_t)d;
            uint64_t q = nn / d;
            if (q != d)
                sum += (seq_t)q;
        }
    }
    return sum;
}

/*  Sequence a(n) defined for n ≥ 1.  */
seq_t a(idx_t n)
{
    /* floor(n/2) */
    seq_t half = (seq_t)(n / 2);
    /* σ(n) */
    seq_t sig = sigma_direct(n);
    /* (half + σ(n)) / n  (truncating division) */
    seq_t numer = half + sig;
    return numer / (seq_t)n;
}
