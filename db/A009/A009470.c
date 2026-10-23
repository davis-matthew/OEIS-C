/*
 * A009470  a(n) is the concatenation of n and 8n.
 * https://oeis.org/A009470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less, easy
 * Formula:   a(n) = concatenate decimal representation of n and 8*n
 * Range:     u8 n <= 2, u16 n <= 65, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_len, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         a(n) = n * 10^digits(8*n) + 8*n                     */
#include <stdbool.h>
#include "../utils/digit_len.h"   /* int digit_len(seq_t x) */
#include "../utils/seq_pow.h"    /* seq_t seq_pow(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    seq_t nn      = (seq_t)n;            /* n as seq_t   */
    seq_t eight_n = nn * (seq_t)8;       /* 8*n          */
    int    d      = digit_len(eight_n);  /* digits of 8*n */
    seq_t pow10   = seq_pow((seq_t)10, (idx_t)d); /* 10^d     */
    return nn * pow10 + eight_n;        /* n*10^d + 8*n */
}
