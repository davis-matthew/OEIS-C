/*
 * A069268  Greatest common divisor of n! and n*(n+1)/2.
 * https://oeis.org/A069268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = if n>1 and n+1 is prime then n/2 else n*(n+1)/2
 * Range:     u8 n <= 22, u16 n <= 361, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* GCD(n!, n*(n+1)/2) simplifies to the above expression. */

#include <stdbool.h>
#include "../utils/isPrime.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }
    if (n > 1 && isPrime(n + 1)) {
        return (seq_t)n / 2;
    }
    return (seq_t)n * ((seq_t)n + 1) / 2;
}
