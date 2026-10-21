/*
 * A095208  n if n is composite else 10*n.
 * https://oeis.org/A095208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, less, nonn
 * Formula:   a(n) = n if n is composite, 10*n otherwise (including n=1)
 * Range:     u8 n <= 28, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Simple deterministic primality test (trial division).  It is fast enough
   for the required n ≤ 10 000 and works for any 64‑bit value. */
static int is_prime(idx_t n)
{
    if (n < 2) return 0;          /* 0 and 1 are non‑prime, 1 is counted as "not composite" */
    if (n % 2 == 0) return n == 2;
    for (idx_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return 0;
    return 1;
}

seq_t a(idx_t n)
{
    if (n == 1 || is_prime(n))
        return (seq_t)((uint64_t)n * 10ULL);
    return (seq_t)n;
}
