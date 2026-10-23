/*
 * A032941  Numbers whose base-6 representation Sum_{i=0..m} d(i)*6^(m-i) has
 *          d(i)=0 for all odd i.
 * https://oeis.org/A032941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑6 representation has digit 0 at all odd
 *            positions (counting from the most significant digit). The n‑th
 *            term is produced by determining the length of its base‑6
 *            representation, choosing the digits on the even positions
 *            (integer‑multiples of 6^2), and converting to decimal
 * Range:     u8 n <= 46, u16 n <= 646, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                       /* may be required for size_t */

static seq_t pow6(idx_t exp) {
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i) r *= 6;
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* offset is 1 – values for n<1 undefined */

    idx_t remaining = n;
    idx_t L = 1;            /* current length of base‑6 representation */
    idx_t k = 1;            /* number of non‑zero even‑position digits = ceil(L/2) */
    idx_t pow6kminus1 = 1;  /* 6^(k-1) */

    /* find the block (length) that contains the n‑th term */
    while (1) {
        idx_t count = 5 * pow6kminus1;   /* numbers of this length */
        if (remaining <= count) break;   /* found the correct length */
        remaining -= count;
        ++L;
        if (L % 2 == 1) {                /* k increases every time L becomes odd */
            ++k;
            pow6kminus1 *= 6;
        }
    }

    /* remaining is 1‑based index inside this length block */
    idx_t idx0 = remaining - 1;          /* zero‑based */
    int digits[128];                     /* array for the k even‑position digits */
    idx_t pow = pow6kminus1;             /* 6^(k-1) */

    /* first digit ranges from 1 to 5 */
    int q = (int)(idx0 / pow);
    digits[0] = q + 1;
    idx_t rem = idx0 - (idx_t)q * pow;

    for (int j = 1; j < (int)k; ++j) {
        pow /= 6;
        int d = (int)(rem / pow);
        digits[j] = d;
        rem -= (idx_t)d * pow;
    }

    /* assemble the base‑6 number with zeros on odd positions */
    seq_t val = 0;
    for (int j = 0; j < (int)k; ++j) {
        idx_t exp = (L - 1) - 2 * (idx_t)j;   /* exponent of 6 for this digit */
        val += (seq_t)digits[j] * pow6(exp);
    }

    return val;
}
