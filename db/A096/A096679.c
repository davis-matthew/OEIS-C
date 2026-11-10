/*
 * A096679  A096787 indexed by A000040.
 * https://oeis.org/A096679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that A000040(k) = A096787(n), i.e. a(n) =
 *            pi(A096787(n)). Computed by a search for the n-th prime p = 2m+1
 *            = 3 (mod 4) with m, m+1 both composite, then a sieve of
 *            Eratosthenes to count the primes <= p
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static bool A096679_isprime(uint64_t n)
{
    if (n < 2) return false;
    if (n < 4) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= n; i += 6)
        if (n % i == 0 || n % (i + 2) == 0) return false;
    return true;
}

static bool A096679_composite(uint64_t n)
{
    return n > 3 && !A096679_isprime(n);
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    idx_t count = 0;
    uint64_t p = 0;
    /* p = 2m+1 = 3 (mod 4) requires m odd */
    for (uint64_t m = 9;; m += 2) {
        if (A096679_composite(m) && A096679_composite(m + 1)) {
            uint64_t q = 2 * m + 1;
            if ((q & 3ULL) == 3 && A096679_isprime(q)) {
                if (++count == n) { p = q; break; }
            }
        }
    }
    unsigned char *comp = (unsigned char *)calloc((size_t)p + 1, 1);
    if (!comp) return (seq_t)0;
    uint64_t k = 0;
    for (uint64_t i = 2; i <= p; i++) {
        if (!comp[i]) {
            k++;
            for (uint64_t j = i * i; j <= p && i * i <= p; j += i)
                comp[j] = 1;
        }
    }
    free(comp);
    return (seq_t)k;
}
