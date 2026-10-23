/*
 * A128464  Numbers that are congruent to {11, 17, 29} mod 30.
 * https://oeis.org/A128464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 30*floor((n-1)/3) + (n-1)%3==0 ? 11 : (n-1)%3==1 ? 17 : 29 */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n=0, return 0 as safe default */
    idx_t k = (n - 1) / 3;        /* quotient part */
    idx_t r = (n - 1) % 3;        /* remainder part (0,1,2) */
    seq_t base;
    switch (r) {                  /* map remainder to appropriate offset */
        case 0:  base = 11; break;
        case 1:  base = 17; break;
        default: base = 29;       /* r == 2 */
    }
    return ((seq_t)k * (seq_t)30) + base;   /* compute the term */
}
