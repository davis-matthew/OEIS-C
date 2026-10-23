/*
 * A167936  a(n) = 2^n - A108411(n).
 * https://oeis.org/A167936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 0, a(1) = 1, a(2) = 1
 * Recurrence:  a(n) = 2*a(n-1) + 3*a(n-2) - 6*a(n-3)  for n ≥ 3
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1 || n == 2) return 1;

    seq_t a0 = 0;          /* a(n-3) */
    seq_t a1 = 1;          /* a(n-2) */
    seq_t a2 = 1;          /* a(n-1) */
    seq_t an = 0;

    for (idx_t i = 3; i <= n; ++i) {
        an = 2 * a2 + 3 * a1 - 6 * a0;  /* compute a(i) */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}
