/*
 * A176972  a(n) = 7^n + 7*n + 1.
 * https://oeis.org/A176972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7^n + 7*n + 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Multiply by 7 n times – works for any seq_t implementation. */
static seq_t pow7(idx_t n)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        res *= (seq_t)7;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t power = pow7(n);                 /* 7^n */
    seq_t mul   = (seq_t)7 * (seq_t)n;     /* 7*n   */
    return power + mul + (seq_t)1;         /* 7^n + 7*n + 1 */
}
