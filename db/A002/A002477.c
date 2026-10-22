/*
 * A002477  Wonderful Demlo numbers: a(n) = ((10^n - 1)/9)^2.
 * https://oeis.org/A002477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000042(n)^2, where A000042(n) = (10^n - 1)/9 is the
 *            repunit 11...1 (n ones)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 0;
    idx_t i;
    for (i = 0; i < n; i++)
        r = r * 10 + 1;
    return r * r;
}
