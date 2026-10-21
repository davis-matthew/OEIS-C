/*
 * A081893  Third binomial transform of C(n+2,2).
 * https://oeis.org/A081893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n * (n^2 + 15*n + 32) / 32
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses only integer arithmetic and works for any
   build of seq_t (uint8_t … uint128_t or an arbitrary‑precision class).
   For the 64‑bit build it avoids intermediate overflow by
   separating the cases n < 3 and n ≥ 3. */
#include <stdint.h>   /* for uint64_t, used only for type definitions */

seq_t a(idx_t n) {
    /* n is an unsigned 64‑bit index.  Convert to seq_t early so
       all subsequent arithmetic is performed with the correct type. */
    seq_t sn = (seq_t)n;

    /* Compute the polynomial P(n) = n^2 + 15*n + 32 */
    seq_t poly = sn * sn + (seq_t)15 * sn + (seq_t)32;

    /* Special handling for n < 3: 4^n is smaller than 32, so
       perform the division after the multiplication. */
    if (n < 3) {
        seq_t pow4 = 1;
        for (idx_t i = 0; i < n; ++i)       /* pow4 = 4^n */
            pow4 *= (seq_t)4;

        /* The product is guaranteed to be divisible by 32. */
        return (poly * pow4) / (seq_t)32;
    }

    /* For n ≥ 3 we can rewrite the formula as:
         a(n) = (n^2 + 15*n + 32) * 2^(2*n – 5)
       The exponent 2*n – 5 is non‑negative, so we can compute
       2^(2*n – 5) by repeated multiplication by 2. */
    idx_t shift = 2 * n - 5;          /* 2*n – 5 >= 1 for n ≥ 3 */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < shift; ++i) /* pow2 = 2^(2*n – 5) */
        pow2 *= (seq_t)2;

    return poly * pow2;
}
