/*
 * A097707  Part of n! composed of prime factors of form 4k+3.
 * https://oeis.org/A097707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p ≤ n, p ≡ 3 (mod 4)} p^{⌊n/p⌋ + ⌊n/p²⌋ + ⌊n/p³⌋ + …}
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 21, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence term a(n) is the product of all prime-power factors of n! coming from
   primes of the form 4k+3.  It is computed here by:
   – generating all primes up to n via a simple sieve,
   – for every such prime with p ≡ 3 (mod 4), computing its exponent in n! by
     Legendre's formula,
   – multiplying the prime to that power into a running product.  All
     arithmetic uses seq_t, which is an arbitrary‑precision integer type in
     the big‑integer build or a native unsigned integer type otherwise.   The
     algorithm runs in O(n / log n) time and uses only O(n) extra storage.  It
     works for any n ≥ 1 and for repeated calls with non‑monotonic n. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)1;           /* offset starts at 1 */

    /* Allocate a bit‑array for the sieve, size n+1.  size_t fits n. */
    size_t sz = (size_t)n + 1;
    unsigned char *is_comp = (unsigned char *)malloc(sz);
    if (!is_comp)                    /* allocation failure: return 1 as safe default */
        return (seq_t)1;
    for (size_t i = 0; i < sz; ++i) is_comp[i] = 0;

    /* Sieve of Eratosthenes to find primes ≤ n. */
    for (size_t i = 2; i * i <= (size_t)n; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= (size_t)n; j += i)
                is_comp[j] = 1;
        }
    }

    seq_t result = (seq_t)1;

    /* For each prime, if it is ≡ 3 (mod 4) compute its exponent in n! and multiply. */
    for (idx_t p = 2; p <= n; ++p) {
        if (!is_comp[(size_t)p] && (p & 3) == 3) { /* p % 4 == 3 */
            /* Legendre's formula for exponent of p in n! */
            idx_t temp = n;
            idx_t exp = 0;
            while (temp) {
                temp /= p;
                exp += temp;
            }

            /* Compute p^exp using binary exponentiation. */
            seq_t term = (seq_t)1;
            seq_t base = (seq_t)p;
            idx_t e = exp;
            while (e) {
                if (e & 1)
                    term *= base;
                base *= base;
                e >>= 1;
            }

            result *= term;
        }
    }

    free(is_comp);
    return result;
}
