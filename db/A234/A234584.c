/*
 * A234584  Let S_n = 0 followed by base-9 expansion of n, reversed; sequence
 *          is concatenation of S_0, S_1, S_2, ...
 * https://oeis.org/A234584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth digit of the concatenation of S_k = 0 +
 *            rev(base9(k)) for k = 0,1,2,…
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   base9_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/base9_digits.h"

/* Return the nth term of the sequence. */
seq_t a(idx_t n)
{
    idx_t pos = n;   /* position within the infinite string */
    idx_t k   = 0;   /* current block index */

    /* Find block containing the position. */
    while (1)
    {
        idx_t len = base9_digits(k) + 1;   /* length of S_k */
        if (pos < len) break;
        pos -= len;
        k++;
    }

    /* Offset within the block. */
    if (pos == 0) return (seq_t)0;         /* leading zero of S_k */

    idx_t e = pos - 1;                     /* index within reversed digits */

    /* Compute 9^e. */
    idx_t pow9 = 1;
    for (idx_t i = 0; i < e; i++) pow9 *= 9ULL;

    /* Digit is the e‑th least significant base‑9 digit of k. */
    seq_t digit = (seq_t)((k / pow9) % 9ULL);
    return digit;
}
