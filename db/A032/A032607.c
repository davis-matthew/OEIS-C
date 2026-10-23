/*
 * A032607  Concatenation of n and n + 2 or {n,n+2}.
 * https://oeis.org/A032607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(n, n+2)
 * Range:     u8 n <= 7, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digit_len.h"

/* Concatenation of n and n+2 in decimal.
 * a(n) = n * 10^{ digits(n+2) } + (n+2)  */
seq_t a(idx_t n)
{
    /* Convert the index to seq_t so arithmetic stays in seq_t */
    seq_t nn  = (seq_t)n;   /* n as seq_t */
    seq_t np2 = nn + 2;     /* n + 2  */

    /* Determine how many decimal digits the second part has */
    idx_t exp = (idx_t)digit_len(np2);

    /* 10^exp as seq_t */
    seq_t pow10_val = pow10(exp);

    /* Concatenate by shifting the first part and adding the second part */
    return nn * pow10_val + np2;
}
