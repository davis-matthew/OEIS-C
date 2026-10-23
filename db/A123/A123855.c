/*
 * A123855  a(n) = Sum_{j=1..n} Sum_{i=1..n} prime(i)^j.
 * https://oeis.org/A123855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..n} Sum_{i=1..n} prime(i)^j
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1 */
    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i) {
        uint32_t p_u = get_prime(i);      /* i‑th prime */
        seq_t p     = (seq_t)p_u;         /* cast to seq_t */
        seq_t pow   = 1;                  /* will hold p^j */
        seq_t term_sum = 0;               /* sum of p^1 .. p^n */
        for (idx_t j = 1; j <= n; ++j) {
            pow   *= p;                   /* p^j */
            term_sum += pow;
        }
        result += term_sum;
    }
    return result;
}
