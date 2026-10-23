/*
 * A027440  TV channels in Tokyo.
 * https://oeis.org/A027440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = 1 (n = 1), * a(n) = 3 (n = 2), * a(n) = 2*(n-1) (n >= 3)
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A027440 – TV channels in Tokyo */
seq_t a(idx_t n)
{
    if (n == 1)
        return 1;
    if (n == 2)
        return 3;
    /* For n >= 3: a(n) = 2*(n-1) */
    return (seq_t)(n - 1) * 2;
}
