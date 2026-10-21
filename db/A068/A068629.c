/*
 * A068629  a(1) = 1. a(n) = n*a(n-1) if gcd(n,a(n-1)) = 1, a(n-1)/n if n
 *          divides a(n-1), otherwise a(n) = a(n-1).
 * https://oeis.org/A068629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 40, u64 n <= 70, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1.
   For n ≥ 2
       if a(n−1) mod n = 0          → a(n) = a(n−1) / n
       else if gcd(a(n−1), n) = 1    → a(n) = a(n−1) * n
       else                           a(n) = a(n−1)   */
#include <stdint.h>
#include <stdbool.h>

/* Euclidean algorithm for arbitrary‐precision type seq_t. */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Compute A068629 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* offset is 1 – 0 is unused */
    if (n == 1) return (seq_t)1;

    seq_t val = (seq_t)1;             /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t inum = (seq_t)i;

        if (val % inum == (seq_t)0) {
            val = val / inum;
        } else if (seq_gcd(val, inum) == (seq_t)1) {
            val = val * inum;
        } /* else leave val unchanged */
    }
    return val;
}
