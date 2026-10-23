/*
 * A143733  Aliquot sequence starting at 62.
 * https://oeis.org/A143733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 62, a(n+1) = A001065(a(n)) – sum of proper divisors of a(n) */
#include <stddef.h>   /* for NULL, size_t if needed */

/* compute sigma(m)-m, the sum of proper divisors of m */
static seq_t sum_of_proper_divisors(seq_t m)
{
    if (m <= 1) return 0;
    seq_t sum = 1;
    for (seq_t i = 2; i <= m / i; ++i) {
        if (m % i == 0) {
            sum += i;
            seq_t other = m / i;
            if (other != i) sum += other;
        }
    }
    return sum;
}

/* aliquot sequence starting at 62 */
seq_t a(idx_t n)
{
    seq_t x = 62;          /* a(0) */
    if (n == 0) return x;

    for (idx_t i = 1; i <= n; ++i) {
        if (x == 0) break;           /* once zero is reached, all further terms are zero */
        x = sum_of_proper_divisors(x);
    }
    return x;
}
