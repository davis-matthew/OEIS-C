/*
 * A088922  Consider the n X n matrix with entries (i*j mod n), where
 *          i,j=0..n-1; a(n) = rank of this matrix over the real numbers.
 * https://oeis.org/A088922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1)/2) + τ(n) - 2, where τ(n) is the divisor
 *            count of n
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the number of positive divisors of n (τ(n)).  This routine uses only
 * integer arithmetic and is efficient enough for n up to at least 1e6.  It
 * works for any 64‑bit value of n. */
static uint64_t divisor_count_uint64(idx_t n)
{
    uint64_t result = 1;
    idx_t exp;
    /* Count prime factor 2 separately */
    exp = 0;
    while ((n & 1) == 0) {
        n >>= 1;
        ++exp;
    }
    if (exp > 0)
        result *= (exp + 1);

    /* Test odd divisors up to sqrt(n) */
    idx_t i = 3;
    while (i <= n / i) {
        if (n % i == 0) {
            exp = 0;
            while (n % i == 0) {
                n /= i;
                ++exp;
            }
            result *= (exp + 1);
        }
        i += 2;
    }

    /* If what's left is a prime > 1 */
    if (n > 1)
        result *= 2;
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* The OEIS offset starts at n = 1.  For safety, handle n < 1. */
    if (n < 1)
        return 0;

    seq_t half = ((seq_t)n + 1) / 2;          /* floor((n+1)/2) */
    uint64_t dcnt = divisor_count_uint64((uint64_t)n);  /* τ(n) */
    seq_t result = half + (seq_t)dcnt - 2;
    return result;
}
