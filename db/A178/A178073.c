/*
 * A178073  Partial sums of sequence A177342.
 * https://oeis.org/A178073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^3+n^2+2*n-1)/3
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 335, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t (unspecified unsigned integer type) */
    seq_t nn = (seq_t) n;

    /* Compute n^2 and n^3 */
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;

    /* Calculate the inner polynomial n^3 + n^2 + 2*n - 1 */
    seq_t inner = n3 + n2;          /* n^3 + n^2 */
    inner += nn << 1;               /* + 2*n   (shift left 1 = multiply by 2) */
    inner -= 1;                     /* - 1      */

    /* Multiply by n and divide by 3 */
    seq_t prod = nn * inner;
    seq_t result = prod / 3;        /* division is exact */

    return result;
}
