/*
 * A179338  Positive integers of the form (10*m^2+1)/11.
 * https://oeis.org/A179338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (110*n*(n-1)+35*(2*n-1)*(-1)^n+39)/4 (OEIS A179338)
 * Range:     u8 n <= 1, u16 n <= 25, u32 n <= 7096, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle offset: sequence starts at n=1. For n=0 return 0. */
    if (n == 0) return 0;

    /* Use seq_t for all arithmetic to avoid overflow in idx_t. */
    seq_t sn = (seq_t)n;
    seq_t term1 = (seq_t)110 * sn * (sn - 1); /* 110*n*(n-1) */
    seq_t num = term1 + 39;                   /* +39 */

    seq_t twon_minus1 = (seq_t)2 * sn - 1;    /* 2*n-1 */
    seq_t add = (seq_t)35 * twon_minus1;      /* 35*(2*n-1) */

    if (n & 1) {              /* odd n: subtract */
        num -= add;
    } else {                  /* even n: add */
        num += add;
    }

    return num / 4;           /* divide by 4 */
}
