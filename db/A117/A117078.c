/*
 * A117078  a(n) is the smallest k such that prime(n+1) = prime(n) + (prime(n)
 *          mod k), or 0 if no such k exists.
 * https://oeis.org/A117078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 6545, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = smallest k > A001223(n) such that A000040(n) % k = A001223(n);
   return 0 if no such k exists.  (A000040 = prime, A001223 = prime gap) */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset starts at 1 */

    seq_t p  = get_prime(n);                  /* prime(n)   */
    seq_t p2 = get_prime(n + 1);              /* prime(n+1) */
    seq_t g  = p2 - p;                        /* gap = prime(n+1)-prime(n) */
    seq_t X  = 2 * p - p2;                    /* X = p - g  */

    seq_t best = 0;
    for (seq_t d = 1; d * d <= X; ++d) {
        if (X % d == 0) {
            seq_t d1 = d;
            seq_t d2 = X / d;
            if (d1 > g && (best == 0 || d1 < best)) best = d1;
            if (d2 > g && (best == 0 || d2 < best)) best = d2;
        }
    }
    return best;
}
