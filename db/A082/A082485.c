/*
 * A082485  Numbers k such that 1/(2-s(k)) is an integer where s(k) =
 *          Sum_{i=1..k} 1/3^floor(sqrt(i)).
 * https://oeis.org/A082485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p^2 - 1 + t * where p = n + k - 1, k is the smallest
 *            non‑negative integer such that * p + 2 <= 3^k <= 3(p + 1) * and
 *            t = 3(p + 1) - 3^k . * This comes from the block representation
 *            of the series * s(k) = Σ_{i=1..k} 1/3^⌊√i⌋ * whose tail after
 *            the end of block p is (p+1)/3^{p-1} . * Solving...
 * Range:     u8 n <= 13, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; behaviour for n<1 is irrelevant */

    /* Find the smallest k such that p+2 <= 3^k <= 3(p+1)   with p = n + k - 1 */
    unsigned long long pow3k = 1;   /* 3^k ;  k <= 64 fits in 64 bits */
    unsigned long long k = 0;
    for (;;) {
        /* Use 128‑bit intermediate to avoid overflow when n is large */
        __uint128_t left  = (__uint128_t)k + (__uint128_t)n + 1;          /* p + 2  */
        __uint128_t right = (__uint128_t)3 * ((__uint128_t)k + (__uint128_t)n); /* 3(p+1) */
        if ((__uint128_t)pow3k >= left && (__uint128_t)pow3k <= right) {
            break;
        }
        ++k;
        pow3k *= 3ULL;
    }

    /* Convert to seq_t and perform all operations in that type */
    seq_t p = (seq_t)n + (seq_t)k - 1;
    seq_t t = (p + 1) * 3U - (seq_t)pow3k;

    /* a(n) = p^2 - 1 + t  ;  p^2 computed in seq_t to avoid overflow */
    seq_t res = p * p - 1 + t;
    return res;
}
