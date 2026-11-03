/*
 * A079791  a(1) = 1, a(n) = a(n-1)/n if a(n-1) is divisible by n else a(n) is
 *          the concatenation of a(n-1) and n.
 * https://oeis.org/A079791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 1 For n > 1: if a(n-1) % n == 0 then a(n) = a(n-1) / n
 *            else a(n) = concatenate(a(n-1), n) The concatenation is
 *            performed in base 10
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 12, u64 n <= 17, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 358 (2026-11-01)
 * Helpers:   num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/num_digits0.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* behaviour below offset is irrelevant */
    seq_t val = 1;                  /* a(1) = 1 */
    if (n == 1) return val;

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t i_seq = (seq_t)i;
        if ((val % i_seq) == 0)
        {
            val /= i_seq;
        }
        else
        {
            /* compute 10^(digits of i) and concatenate */
            uint64_t digs = num_digits0((uint64_t)i);
            seq_t pow = pow10((idx_t)digs);
            val = val * pow + i_seq;
        }
    }
    return val;
}
