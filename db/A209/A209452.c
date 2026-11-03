/*
 * A209452  a(n) = Pell(n)*A122859(n) for n>=1, with a(0)=1, where A122859
 *          lists the coefficients in phi(-q)^3/phi(-q^3) and phi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A209452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Pell(n) * A122859(n) where A122859(n) = -6 * sum_{d|n}
 *            Kronecker(d,3) * (-1)^{n/d - 1} Kronecker(d,3) = 0 if 3|d, 1 if
 *            d ≡ 1 (mod 3), -1 if d ≡ 2 (mod 3)
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 23, i64 n <= 48, i128 n <= 97,
 *            big any n
 * Validated: standard, pass, 53 terms through n = 984 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Helper: Kronecker symbol (d,3) */
static inline seq_t kronecker3(idx_t d)
{
    if (d % 3 == 0) return (seq_t)0;
    return (seq_t)(d % 3 == 1 ? 1 : -1);
}

/* Helper: Pell(n) for n >= 0 */
static seq_t pell(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    seq_t p_prev = (seq_t)0;    /* Pell(0) */
    seq_t p_curr = (seq_t)1;    /* Pell(1) */
    seq_t p_next;
    for (idx_t i = 2; i <= n; ++i) {
        p_next = p_curr * (seq_t)2 + p_prev;
        p_prev = p_curr;
        p_curr = p_next;
    }
    return p_curr;
}

/* Main function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;      /* offset */

    /* Compute A122859(n) */
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; (idx_t)(d * d) <= n; ++d) {
        if (n % d != 0) continue;
        idx_t e = n / d;

        seq_t k = kronecker3(d);
        if (k != 0) {
            seq_t sign = (e % (idx_t)2 == 0) ? (seq_t)-1 : (seq_t)1;
            sum += k * sign;
        }

        if (e == d) continue; /* avoid double counting square divisors */

        seq_t k2 = kronecker3(e);
        if (k2 != 0) {
            seq_t sign2 = (d % (idx_t)2 == 0) ? (seq_t)-1 : (seq_t)1;
            sum += k2 * sign2;
        }
    }

    seq_t a122859 = sum * (seq_t)-6;

    /* Multiply by Pell(n) */
    return pell(n) * a122859;
}
