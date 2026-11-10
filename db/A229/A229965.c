/*
 * A229965  Numbers n such that A229964(n) = 1.
 * https://oeis.org/A229965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th smallest element of {6, 8, 16} UNION A156592, where
 *            A156592 = { p*(2p+1) : p and 2p+1 both prime } (the Sophie
 *            Germain prime products); computed by walking the Sophie Germain
 *            primes in increasing order and merging in the three extra terms
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 37 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A229965_isprime(uint64_t m)
{
    if (m < 2) return false;
    if (m % 2 == 0) return m == 2;
    if (m % 3 == 0) return m == 3;
    for (uint64_t d = 5; d * d <= m; d += 6) {
        if (m % d == 0 || m % (d + 2) == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    static const uint64_t extras[3] = { 6, 8, 16 };
    int ei = 0;
    idx_t count = 0;
    uint64_t p = 1;

    for (;;) {
        /* next Sophie Germain prime p */
        do {
            ++p;
        } while (!(A229965_isprime(p) && A229965_isprime(2 * p + 1)));
        uint64_t prod = p * (2 * p + 1);

        /* emit any extra terms smaller than this product first */
        while (ei < 3 && extras[ei] < prod) {
            ++count;
            if (count == n) return (seq_t)extras[ei];
            ++ei;
        }
        ++count;
        if (count == n) return (seq_t)p * ((seq_t)p * 2 + 1);
    }
}
