/*
 * A078152  a(n) = A055086(n) - A000005(n).
 * https://oeis.org/A078152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ floor(n/j) : 1 ≤ j ≤ n }| – τ(n)
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where τ(n) = number of divisors of n. */
/* The function computes the number of distinct quotient values of n/j,
   subtracts the divisor count, and returns the result. */

#include <stdint.h>

static seq_t divisor_count(idx_t n) {
    idx_t tmp = n;
    seq_t cnt = 1;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            seq_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            cnt *= (exp + 1);
        }
    }
    if (tmp > 1)
        cnt *= 2;
    return cnt;
}

seq_t a(idx_t n) {
    seq_t distinct = 0;
    for (idx_t i = 1; i <= n; ) {
        idx_t q = n / i;
        ++distinct;
        i = n / q + 1;
    }
    seq_t divs = divisor_count(n);
    return distinct - divs;
}
