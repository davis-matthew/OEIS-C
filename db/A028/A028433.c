/*
 * A028433  Golc sequence in base 3. Left to right concatenation of
 *          n,int(log_3(n)),int(log_3(int(log_3(n)))),... in base 3.
 * https://oeis.org/A028433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is obtained by writing n in base‑3 followed by the integer
 *            base‑3 logarithms of n (floor(log_3(n)),
 *            floor(log_3(floor(log_3(n)))), …). The concatenation stops just
 *            before a zero that would follow a 1. For example: a(1) =
 *            "1""0"_3 = 3 a(3) = "10""1"_3 = 10 a(9) = "100""2""0"_3 = 87
 * Range:     u8 n <= 26, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The below implementation uses only integer arithmetic.  */
#include <stdint.h>

/* Compute floor(log_3(n)) for unsigned n. */
static inline idx_t floor_log3(idx_t n)
{
    idx_t p = 1;
    idx_t e = 0;
    /* Stop multiplication when it would overflow the range of n/3. */
    while (p <= n / 3) {
        p *= 3;
        ++e;
    }
    return e;
}

/* Compute 3^exp as a seq_t. */
static inline seq_t pow3_exp(idx_t exp)
{
    seq_t r = 1;
    while (exp--) r *= 3;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety – not in the OEIS domain */

    idx_t k = floor_log3(n);        /* floor(log_3(n)) */

    /* CASE k == 0 : n, 0   → result = n * 3 + 0 */
    if (k == 0) {
        return (seq_t)n * 3;
    }
    /* CASE k == 1 : n, 1   → result = n * 3 + 1 */
    if (k == 1) {
        return (seq_t)n * 3 + 1;
    }
    /* CASE k == 2 : n, 2, 0  → result = n * 9 + (2*3 + 0) */
    if (k == 2) {
        return (seq_t)n * 9 + 6;    /* 2*3 + 0 = 6 */
    }
    /* CASE k >= 3 : n, k, 1  (k has at least 2 base‑3 digits) */
    {
        idx_t digits_k = floor_log3(k) + 1;          /* number of base‑3 digits of k */
        seq_t shift = pow3_exp(digits_k + 1);        /* 3^(digits_k+1)   */
        return (seq_t)n * shift + (seq_t)k * 3 + 1;  /* n*shift + k*3 + 1 */
    }
}
