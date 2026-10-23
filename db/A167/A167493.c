/*
 * A167493  a(1) = 2; thereafter a(n) = a(n-1) + gcd(n, a(n-1)) if n is odd,
 *          and a(n) = a(n-1) + gcd(n-2, a(n-1)) if n is even.
 * https://oeis.org/A167493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 132, u16 n <= 9561, u32 n <= 9561, u64 n <= 9561,
 *            u128 n <= 9561, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(1) = 2; for n ≥ 2,
 *          a(n) = a(n-1) + gcd(n, a(n-1))   if n is odd
 *          a(n) = a(n-1) + gcd(n-2, a(n-1)) if n is even
 */

#include "../utils/gcd.h"          /* seq_t gcd(seq_t, seq_t) */

seq_t a(idx_t n)
{
    /* n is a 1‑based index, n ≥ 1 */
    if (n == 0)          /* undefined, but return 0 for safety */
        return 0;

    seq_t prev = 2;      /* a(1) */

    if (n == 1)
        return prev;

    /* Compute terms up to a(n) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t add;
        if (i & 1ULL) {                     /* i is odd */
            add = gcd((seq_t)i, prev);
        } else {                            /* i is even */
            add = gcd((seq_t)(i - 2), prev);
        }
        prev = prev + add;
    }

    return prev;
}
