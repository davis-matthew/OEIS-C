/*
 * A143846  Aliquot sequence starting at 88.
 * https://oeis.org/A143846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 88, a(n+1) = A001065(a(n)) (sum of proper divisors)
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root: largest r such that r*r <= n.
   Works for any seq_t that supports the required operators. */
static inline seq_t int_sqrt(seq_t n)
{
    if (n < (seq_t)2) return n;
    seq_t low = 1;
    seq_t high = n >> 1;            /* floor(n/2) is always >= sqrt(n) for n >= 2 */
    while (low <= high) {
        seq_t mid = (low + high + 1) / 2;   /* upper mid */
        /* check mid*mid <= n without overflow: mid <= n/mid */
        if (mid <= n / mid) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return low - 1;   /* low is first r with r*r > n */
}

/* Sum of proper divisors of n. */
static seq_t sum_proper_divisors(seq_t n)
{
    if (n <= (seq_t)1) return (seq_t)0;  /* 0 and 1 have no proper divisors */

    seq_t sum = (seq_t)0;
    seq_t lim = int_sqrt(n);

    /* Iterate i from 1 to lim.  cast to idx_t for the loop counter
       which is guaranteed to fit because lim <= max(idx_t) for the
       intended use range (starting from 88). */
    for (idx_t i = 1; i <= (idx_t)lim; ++i) {
        seq_t d = (seq_t)i;
        if (n % d == (seq_t)0) {
            seq_t other = n / d;
            if (d == other) {
                sum += d;          /* perfect square divisor */
            } else {
                sum += d + other;  /* complementary pair */
            }
        }
    }
    /* Remove n itself to obtain proper divisors sum. */
    sum -= n;
    return sum;
}

/* Sequence a(n).  Computation is O(n * sqrt(a(i))) which is fine for n <= 10000. */
seq_t a(idx_t n)
{
    static const seq_t START = (seq_t)88;      /* a(0) = 88 */
    seq_t cur = START;

    if (n == 0) return cur;

    for (idx_t i = 1; i <= n; ++i) {
        cur = sum_proper_divisors(cur);
        if (cur == (seq_t)0) {                 /* once zero, stays zero */
            return cur;
        }
    }
    return cur;
}
