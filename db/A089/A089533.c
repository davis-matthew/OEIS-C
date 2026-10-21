/*
 * A089533  a(n)=(A089348(n)-1)/10n.
 * https://oeis.org/A089533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A089348(n)-1)/(10n)
 * Range:     u8 n <= 10, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A089348(n) is the smallest prime congruent to 1 modulo 10n;  */
/* therefore a(n) is the smallest k such that 10*n*k + 1 is prime. */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1, but guard against 0 */
    const seq_t step = (seq_t)10 * n; /* 10*n as a seq_t value */
    for (idx_t k = 1; ; ++k) {
        seq_t candidate = step * (seq_t)k + 1;
        if (is_prime_big(candidate))
            return (seq_t)k;
    }
}
