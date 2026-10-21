/*
 * A232488  a(1) = 7; a(n+1) = a(n) + product of nonzero digits of a(n).
 * https://oeis.org/A232488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 7; a(n+1) = a(n) + product of non‑zero digits of a(n)
 * Range:     u8 n <= 20, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   digit_counts, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/digit_counts.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

/*  Compute the product of the non‑zero decimal digits of x.
    The function relies on digit_counts to count each digit.  */
static seq_t nonzero_digit_product(seq_t x)
{
    unsigned char cnt[10];
    digit_counts(x, cnt);               /* cnt[0] … cnt[9] */

    seq_t prod = (seq_t)1;
    for (int d = 1; d <= 9; ++d) {
        if (cnt[d] == 0) continue;
        seq_t pd = seq_pow((seq_t)d, (idx_t)cnt[d]);  /* d^cnt[d] */
        prod = seq_mul(prod, pd);
    }
    return prod;                       /* product of non‑zero digits   */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0, safety   */
    seq_t cur = (seq_t)7;                 /* a(1) = 7                     */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t prod = nonzero_digit_product(cur);
        cur = cur + prod;                 /* a(i) = a(i‑1) + product      */
    }
    return cur;
}
