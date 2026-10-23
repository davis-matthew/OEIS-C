/*
 * A048297  Coefficients in power series expansion over GF(2)[ X^(-1) ] of
 *          continued fraction [ 0, X, X^2, X^4, X^8, X^16, ... ].
 * https://oeis.org/A048297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = 3*A003714(n)+1  where A003714(n) is the n‑th nonnegative integer
   whose binary representation has no consecutive 1’s.
   Implementation:  unrank the binary strings of length len with leading 1
   using Fibonacci numbers as counts.  Complexity O(log n) per call.
*/

#include <stdint.h>
#include <stdbool.h>

#define FIB_MAX 90          /* enough for n up to many millions */

/* Fibonacci numbers used for counting binary strings without consecutive 1's. */
static idx_t fib[FIB_MAX + 1];
static bool fib_ready = false;

static void init_fib(void)
{
    if (fib_ready) return;
    fib[0] = 0;
    fib[1] = 1;
    for (idx_t i = 2; i <= FIB_MAX; ++i)
        fib[i] = fib[i-1] + fib[i-2];
    fib_ready = true;
}

seq_t a(idx_t n)
{
    /* a(0) = 1 (the value corresponding to the binary number 0). */
    if (n == 0)
        return (seq_t)1;

    init_fib();

    /* rank among the positive numbers (exclude 0) */
    idx_t k = n - 1;
    idx_t cumulative = 0;
    idx_t len = 1;

    /* find the bit length of the target number */
    while ((cumulative + fib[len]) <= k) {
        cumulative += fib[len];
        ++len;
    }

    idx_t offset = k - cumulative;           /* position within this length */

    unsigned long long val = 1ULL << (len - 1);   /* leading 1 */

    for (int pos = (int)(len - 2); pos >= 0; --pos) {
        idx_t remaining = (idx_t)pos;            /* bits after this position */
        idx_t count_if0 = fib[remaining + 2];    /* strings if this bit = 0 */

        if (offset >= count_if0) {
            offset -= count_if0;
            val |= 1ULL << pos;                 /* set bit to 1 */
        }
    }

    return (seq_t)(val * 3ULL + 1ULL);
}
