/*
 * A157732  a(n) = 388962*n^2 - 430416*n + 119071.
 * https://oeis.org/A157732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 388962*n^2 - 430416*n + 119071
 * Range:     u8 none, u16 none, u32 n <= 105, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1 (so a(1) is the first term listed in OEIS) */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (seq_t)388962 * nn * nn
         - (seq_t)430416 * nn
         + (seq_t)119071;
}
