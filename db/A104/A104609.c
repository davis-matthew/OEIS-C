/*
 * A104609  Write the natural numbers in base 5 in a triangle with k digits in
 *          the k-th row, as shown below. Sequence gives the leading diagonal.
 * https://oeis.org/A104609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = digit at position n(n+1)/2 in the stream of base‑5 digits
 *            of the natural numbers
 * Range:     u8 n <= 12, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* undefined, safe fallback */

    /* Position of the digit in the infinite stream (1‑based). */
    seq_t p = (seq_t)n * ((seq_t)n + 1) / 2;

    /* Find the digit length d of the number that contains this position. */
    seq_t sum  = 0;          /* total digits processed so far */
    seq_t pow5 = 1;          /* 5^(d-1) */
    int d = 1;               /* digit length */

    for (;;) {
        seq_t count = 4 * pow5;                 /* numbers with d digits */
        seq_t block = count * (seq_t)d;         /* digits contributed by this block */
        if (sum + block >= p)
            break;
        sum += block;
        pow5 *= 5;
        ++d;
    }

    /* Position inside the block for this digit. */
    seq_t offset   = p - sum - 1;                /* 0‑based offset in the block */
    seq_t numIndex = offset / (seq_t)d;          /* index of the d‑digit number (0‑based) */
    int digitIdx   = (int)(offset % (seq_t)d);    /* digit position from the left (0‑based) */

    /* Decimal value of the d‑digit base‑5 number. */
    seq_t number = pow5 + numIndex;

    /* Divisor 5^(d-1-digitIdx) used to extract the desired digit. */
    seq_t divisor = pow5;
    for (int i = 0; i < digitIdx; ++i) {
        divisor /= 5;
    }

    seq_t digit = (number / divisor) % 5;
    return digit;
}
