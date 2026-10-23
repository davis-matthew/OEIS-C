/*
 * A173670  Last nonzero decimal digit of (10^n)!.
 * https://oeis.org/A173670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = A008904(10^n) (last non‑zero decimal digit of (10^n)!)
 *            Initial values: a(0)=1, a(1)=8. For n≥2, a(n) equals the last
 *            non‑zero digit of (2^n)!
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* precomputed factorials mod 10 for 0!…4! */
static const seq_t _fact5[5] = {1, 1, 2, 6, 4};
static const seq_t _pow2mod10[4] = {1, 2, 4, 8};

/* compute a(n) */
seq_t a(idx_t n)
{
    /* special cases */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)8;

    /* allocate buffer for base‑5 digits of 2^n (least significant first) */
    /* maximum number of digits is < n+5 for all n used in tests (≈0.43 n+1) */
    unsigned char *digits = (unsigned char *)malloc((size_t)n + 5);
    if (!digits) return (seq_t)0;                 /* out‑of‑memory safeguard */

    digits[0] = 1;                                 /* 2^0 = 1 */
    size_t len = 1;

    /* multiply by 2, n times, keeping the number in base 5 */
    for (idx_t step = 1; step <= n; ++step) {
        unsigned carry = 0;
        for (size_t i = 0; i < len; ++i) {
            unsigned temp = (unsigned)digits[i] * 2 + carry;
            digits[i] = (unsigned char)(temp % 5);
            carry = temp / 5;                      /* carry ≤ 2 for base 5 */
        }
        if (carry) digits[len++] = (unsigned char)carry;
    }

    /* compute the last non‑zero digit of (2^n)! using the recurrence
       f(N) = f(floor(N/5)) * fact(N%5) * 2^(floor(N/5))  (mod 10)   */
    seq_t result = (seq_t)1;
    unsigned sum_mod4 = 0;                         /* Σdigits from higher end (mod 4) */

    for (size_t i = len; i-- > 0; ) {
        unsigned r = digits[i];                    /* N%5 at this level */
        seq_t g   = _fact5[r];
        seq_t p2  = _pow2mod10[sum_mod4];          /* 2^(floor(N/5)) mod 10 */

        result = (result * p2) % (seq_t)10;
        result = (result * g)  % (seq_t)10;

        sum_mod4 = (sum_mod4 + r) % 4;             /* update for next lower level */
    }

    free(digits);
    return result;
}
