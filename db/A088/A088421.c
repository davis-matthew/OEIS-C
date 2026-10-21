/*
 * A088421  Number of primes in arithmetic progression starting with 5 and
 *          with d=2n.
 * https://oeis.org/A088421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the count of primes in the arithmetic progression 5,
 *            5+2n, 5+2·2n, 5+2·3n, ... until the first non‑prime appears
 * Range:     u8 n <= 41, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined for n=0, return 0 */
    seq_t d   = (seq_t)n * 2;                  /* common difference 2n   */
    idx_t  cnt = 0;                              /* number of consecutive primes   */
    while (true) {
        seq_t val = 5 + (seq_t)cnt * d;         /* current term of the AP       */
        if (!is_prime((idx_t)val)) break;       /* stop at first composite      */
        cnt++;
    }
    return (seq_t)cnt;
}
