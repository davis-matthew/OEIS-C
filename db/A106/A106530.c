/*
 * A106530  Least common multiple of all parts in Zeckendorf representation of
 *          n.
 * https://oeis.org/A106530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = lcm of parts in Zeckendorf representation of n
 * Range:     u8 n <= 44, u16 n <= 138, u32 n <= 981, u64 n <= 9352,
 *            u128 n <= 9352, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Euclidean algorithm for seq_t */
static inline seq_t my_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* n >= 1 according to the offset */
    seq_t rem = (seq_t)n;
    /* generate Fibonacci numbers 1,2,3,5,... up to n */
    seq_t fib[64];
    size_t cnt = 0;
    /* initial Fibonacci numbers */
    fib[cnt++] = 1;   /* F1 */
    if (n >= 2) fib[cnt++] = 2;   /* F2 */
    seq_t a1 = 1, a2 = 2;
    while (1) {
        seq_t next = a1 + a2;
        if (next > rem) break;
        fib[cnt++] = next;
        a1 = a2;
        a2 = next;
    }

    seq_t lcm = 0;
    for (size_t i = cnt; i-- > 0; ) {
        if (rem >= fib[i]) {
            rem -= fib[i];
            seq_t part = fib[i];
            if (lcm == 0) {
                lcm = part;
            } else {
                seq_t g = my_gcd(lcm, part);
                lcm = (lcm / g) * part;
            }
        }
    }
    return lcm;
}
