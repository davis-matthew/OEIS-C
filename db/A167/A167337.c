/*
 * A167337  Totally multiplicative sequence with a(p) = 2*(5p+1) = 10p+2 for
 *          prime p.
 * https://oeis.org/A167337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167337: Totally multiplicative with a(p) = 10p + 2.
   For composite n, a(n) = ∏_{p^e‖n} (10p + 2)^e. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) = 1; for any n >= 1 */
    if (n <= 1) return 1;

    uint64_t t = (uint64_t)n;
    seq_t result = 1;

    /* factorise t by trial division – fast enough for n ≤ 10000 */
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            seq_t factor = (seq_t)10 * (seq_t)p + 2;
            seq_t power = 1;
            for (int i = 0; i < e; ++i) power *= factor;
            result *= power;
        }
    }
    /* t > 1 now means t is prime */
    if (t > 1) {
        seq_t factor = (seq_t)10 * (seq_t)t + 2;
        result *= factor;
    }

    return result;
}
