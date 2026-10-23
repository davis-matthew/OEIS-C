/*
 * A010786  Floor-factorial numbers: a(n) = Product_{k=1..n} floor(n/k).
 * https://oeis.org/A010786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = \prod_{k=1}^n \lfloor n/k \rfloor
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 31, u64 n <= 59, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Multiply ``base`` by itself ``exp`` times using repeated squaring. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* a(0) is defined as 1 */
    if (n == 0)
        return (seq_t)1;

    seq_t ans = (seq_t)1;
    idx_t k = 1;

    while (k <= n) {
        idx_t v = n / k;           /* current value of floor(n/k) */
        idx_t next_k = n / v + 1;  /* first k where the floor changes */
        idx_t cnt = next_k - k;    /* how many times v appears consecutively */

        ans *= pow_seq((seq_t)v, cnt);
        k = next_k;
    }
    return ans;
}
