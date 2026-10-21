/*
 * A239910  Number of forests with three connected components in the complete
 *          graph K_{n}.
 * https://oeis.org/A239910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n‑6)·(n‑1)·(n‑2)·(n²+13·n+60)/8 for n ≥ 6; initial
 *            values: a(1)=0, a(2)=0, a(3)=1, a(4)=6, a(5)=45
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the first five terms explicitly (formula not valid for n<6). */
    if (n == 1) return 0;
    if (n == 2) return 0;
    if (n == 3) return 1;
    if (n == 4) return 6;
    if (n == 5) return 45;

    /* For n ≥ 6 the closed‑form formula applies. */
    seq_t n1   = (seq_t)n;                      /* base */
    seq_t exp  = (seq_t)(n - 6);                /* exponent (n‑6) */

    seq_t power = big_pow(n1, exp);             /* n^(n-6) */

    seq_t val = power;
    val = val * (n1 - 1);                       /* * (n-1) */
    val = val * (n1 - 2);                       /* * (n-2) */

    seq_t poly = n1 * n1 + (seq_t)13 * n1 + (seq_t)60; /* n²+13n+60 */
    val = val * poly;                           /* * polynomial */

    val = val / 8;                               /* exact division by 8 */

    return val;
}
