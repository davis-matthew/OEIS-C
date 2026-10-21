/*
 * A079823  Consider the triangle shown below; sequence contains the
 *          concatenation of numbers read at a 45-degree angle upwards with
 *          horizontal beginning with the first term of a row.
 * https://oeis.org/A079823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = decimal concatenation of the * anti‑diagonal starting at
 *            position (n, 1) in the triangular array * * t(r,c) = ((r-1)*r)/2
 *            + c (1 ≤ c ≤ r) * * The diagonal consists of the elements *
 *            t(n,1), t(n-1,2), …, t(n-⌈n/2⌉+1,⌈n/2⌉). The result is obtained
 *            * by appending the decimal representations o...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* count decimal digits of a seq_t value */
static unsigned digit_count(seq_t v) {
    if (v == 0) return 1;
    unsigned d = 0;
    while (v >= 10) {
        v /= 10;
        ++d;
    }
    return d + 1;
}

/* multiply by 10^d */
static seq_t pow10(uint32_t d) {
    seq_t p = 1;
    while (d--) p *= 10;
    return p;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                  /* offset 1; behaviour below is irrelevant */

    idx_t len = (n + 1) / 2;                /* number of diagonal elements */
    seq_t result = 0;

    for (idx_t k = 0; k < len; ++k) {
        /* coordinates of the k-th element on the diagonal */
        idx_t r = n - k;          /* row */
        idx_t c = 1 + k;          /* column */

        /* value at (r,c) */
        seq_t val = ((seq_t)(r - 1) * (seq_t)r) / 2 + (seq_t)c;

        /* append val to result */
        unsigned d = digit_count(val);
        seq_t mult = pow10(d);
        result = result * mult + val;
    }

    return result;
}
