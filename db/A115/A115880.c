/*
 * A115880  Largest positive x satisfying the Diophantine equation x^2 =
 *          y*(y+n), a(n)=0 if there are no solutions.
 * https://oeis.org/A115880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest x such that x^2 = y(y+n). Computed by closed
 *            forms depending on n mod 4: a(4*j) = j^2 - 1 a(4*j+1) = 4*j^2 +
 *            2*j a(4*j+2) = 2*j^2 + 2*j a(4*j+3) = 4*j^2 + 6*j + 2
 * Range:     u8 n <= 32, u16 n <= 512, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t j = (seq_t)(n / 4);        /* base block number */
    switch (n % 4) {
        case 0: return j * j - 1;           /* a(4*j)   */
        case 1: return 4 * j * j + 2 * j;   /* a(4*j+1) */
        case 2: return 2 * j * j + 2 * j;   /* a(4*j+2) */
        default: return 4 * j * j + 6 * j + 2; /* a(4*j+3)*/
    }
}
