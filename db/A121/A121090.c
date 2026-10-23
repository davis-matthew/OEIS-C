/*
 * A121090  Period of unit fractions having periodic decimal expansions.
 * https://oeis.org/A121090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A007732(A085837(n))
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Period of the unit fraction 1/k where k is the n‑th integer ≥3 having a prime factor other than 2 or 5. */

#include <stdint.h>

/* Return the n‑th term of the sequence.  n >= 1. */
seq_t a(idx_t n)
{
    /* Step 1: find the n‑th valid denominator k (≥3) that has a prime factor ≠ 2,5. */
    uint64_t k = 3;           /* candidate integer */
    idx_t    cnt = 0;         /* how many valid numbers have been seen */

    for (;; ++k) {
        uint64_t t = k;
        while (t % 2ULL == 0) t /= 2ULL;
        while (t % 5ULL == 0) t /= 5ULL;
        if (t > 1) {          /* k has a prime factor other than 2 or 5 */
            ++cnt;
            if (cnt == n)     /* n‑th such number found */
                break;
        }
    }

    /* Step 2: remove all factors 2 and 5 from k to obtain the period base. */
    seq_t d = k;
    while (d % 2 == 0) d /= 2;
    while (d % 5 == 0) d /= 5;

    /* For safety, if d==1 (should not happen) return 0. */
    if (d == 1) return (seq_t)0;

    /* Step 3: compute the multiplicative order of 10 modulo d. */
    seq_t rem    = 10 % d;   /* 10^1 mod d */
    seq_t period = 1;

    while (rem != 1) {
        rem = (rem * 10) % d;
        ++period;
    }

    return period;
}
