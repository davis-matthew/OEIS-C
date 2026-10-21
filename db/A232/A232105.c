/*
 * A232105  Number of groups of order prime(n)^5.
 * https://oeis.org/A232105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of groups of order prime(n)^5. For the n-th prime
 *            p: if p = 2 → 51 if p = 3 → 67 otherwise a(n) = 61 + 2p +
 *            2·gcd(p−1,3) + gcd(p−1,4) (OEIS A232105)
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* n is 1‑based index. For n < 1 the behaviour is unspecified. */
    if (n < 1) return 0;

    /* fetch the n-th prime */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    if (p32 == 2) return (seq_t)51;
    if (p32 == 3) return (seq_t)67;

    seq_t p_minus1 = p - (seq_t)1;
    seq_t g3 = gcd(p_minus1, (seq_t)3);
    seq_t g4 = gcd(p_minus1, (seq_t)4);

    seq_t result = (seq_t)61 + (seq_t)2 * p + (seq_t)2 * g3 + g4;
    return result;
}
