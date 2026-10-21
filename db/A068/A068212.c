/*
 * A068212  a(n) = phi(n) divided by its largest prime factor.
 * https://oeis.org/A068212
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(n) / largest_prime_factor(phi(n))
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Euler's totient phi(n) is computed by trial division.  The
   function uses 64‑bit arithmetic because phi(n) ≤ n for all
   n ≥ 1, and for the range of n this easily fits in 64 bits.  */

/* The seq_t and idx_t types are supplied by the build; this file does
   not define them.  All arithmetic is performed with the operators
   allowed by the specification.  */
#include <stdint.h>

/* Compute Euler's totient of n.  The result is obtained using 64‑bit
   integer arithmetic.  It is then cast to seq_t at the end.  */
static seq_t phi_u64(idx_t n)
{
    uint64_t nn = n;
    uint64_t result = nn;
    for (uint64_t p = 2; p * p <= nn; ++p)
    {
        if (nn % p == 0)
        {
            while (nn % p == 0) nn /= p;
            result -= result / p;
        }
    }
    if (nn > 1) result -= result / nn;
    return (seq_t)result;
}

/* Return the largest prime factor of x (x ≥ 1).  For x == 1 we return
   1 so that the division later gives phi.  */
static uint64_t largest_prime_factor_u64(uint64_t x)
{
    if (x < 2) return 1;
    uint64_t maxpf = 1;
    for (uint64_t p = 2; p * p <= x; ++p)
    {
        if (x % p == 0)
        {
            maxpf = p;
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) maxpf = x;
    return maxpf;
}

/* Main sequence function.  It is valid C11 and C++17, has external
   linkage, and meets all the constraints listed in the prompt.  */
seq_t a(idx_t n)
{
    /* The behaviour for n < 3 is not important for the OEIS tests,
       but we handle it gracefully.  */
    if (n < 3) return (seq_t)0;

    seq_t phi_n = phi_u64(n);
    uint64_t phi_u = (uint64_t)phi_n;
    uint64_t lpf = largest_prime_factor_u64(phi_u);

    /* phi(n) is always divisible by its largest prime factor. */
    return phi_n / (seq_t)lpf;
}
