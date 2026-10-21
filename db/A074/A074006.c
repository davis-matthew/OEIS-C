/*
 * A074006  Number of elements of GF(5^n) with trace 0 and subtrace 0.
 * https://oeis.org/A074006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 5^(n-2) for all n, plus a correction * for odd n ≥ 7: add
 *            (-1)^{floor((n-1)/4)} * 4 * 5^((n-3)/2) * for even n that are ≡
 *            2 (mod 8) (i.e., n/2 ≡ 1 (mod 4), n ≥ 10): subtract 4 *
 *            5^(n/2-1) * Small cases: a(1)=a(2)=a(3)=1, a(4)=25, a(5)=125,
 *            a(6)=625
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute 5^exp as seq_t */
static inline seq_t pow5(idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)5;
    while (exp)
    {
        if (exp & (idx_t)1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Small pre‑computed values for n = 1 … 6 */
    static const seq_t small[6] = {
        (seq_t)1,  /* n = 1 */
        (seq_t)1,  /* n = 2 */
        (seq_t)1,  /* n = 3 */
        (seq_t)25, /* n = 4 */
        (seq_t)125,/* n = 5 */
        (seq_t)625 /* n = 6 */
    };
    if (n <= 6) return small[n-1];

    seq_t base = pow5(n-2);          /* 5^(n-2) */

    if ((n & 1) == 0) {              /* even n */
        idx_t half = n / 2;          /* n/2 */
        if ((half & 3) == 1) {       /* half ≡ 1 (mod 4), i.e., n ≡ 2 (mod 8) */
            seq_t delta = (seq_t)4 * pow5(half - 1); /* 4 * 5^(n/2-1) */
            return base - delta;
        } else {
            return base;
        }
    } else {                         /* odd n (n ≥ 7) */
        seq_t delta = (seq_t)4 * pow5((n-3)/2); /* 4 * 5^((n-3)/2) */
        idx_t k = (n-1) / 4;                 /* floor((n-1)/4) */
        if (k & 1) {                         /* sign = -1 */
            return base - delta;
        } else {                             /* sign = +1 */
            return base + delta;
        }
    }
}
