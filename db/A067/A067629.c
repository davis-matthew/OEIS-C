/*
 * A067629  The average of the prime factors of n, rounded off to the nearest
 *          integer (rounding up if there's a choice), with each factor
 *          weighted according to its frequency of occurrence in the prime
 *          factorization.
 * https://oeis.org/A067629
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ⌊( ∑ p_i·e_i + (∑ e_i)/2 ) / (∑ e_i) ⌋ * where n = ∏
 *            p_i^e_i is the prime factorisation of n. * (This implements the
 *            integer‑round‑to‑nearest‑with‑half‑up rule.) * For n < 2 the
 *            function returns 0 (the offset of the sequence is 2)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 2. */
    if (n < 2u) return 0;

    uint64_t x = (uint64_t)n;      /* unsigned 64‑bit copy of n  */
    uint64_t sum  = 0;             /* Σ p_i·e_i                           */
    uint64_t total = 0;            /* Σ e_i                               */

    /* Trial‑division factorisation.  For n ≤ 10 000 this is trivial,
       and for larger n it remains fast enough for the 2‑second
       limit.  The loop uses division by p to avoid overflow when
       p*p would exceed 64 bits. */
    for (uint64_t p = 2; p <= x / p; ++p) {
        if (x % p == 0) {
            uint64_t exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            sum   += p * exp;
            total += exp;
        }
    }

    /* Remaining prime factor > 1 */
    if (x > 1) {
        sum   += x;     /* exponent is 1 */
        total += 1;
    }

    /* Rounding half‑up: (sum + total/2) / total */
    uint64_t result = (sum + total / 2) / total;
    return (seq_t)result;
}
