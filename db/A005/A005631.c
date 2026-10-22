/*
 * A005631  Bishops on a 2n+1 X 2n+1 board (see Robinson paper for details).
 * https://oeis.org/A005631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000898(ceil(n/2)) * A000898(floor(n/2)+1) / 2, where
 *            A000898(k) = 2*A000898(k-1) + 2*(k-1)*A000898(k-2), A000898(0) =
 *            1, A000898(1) = 2. With c(k) = A000898(k)/2 for k >= 1: c(1) =
 *            1, c(2) = 3, c(k) = 2*c(k-1) + 2*(k-1)*c(k-2); then a(0) = 1 and
 *            a(n) = 2*c(ceil(n/2))*c(floor(n/2)+1) for n...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t i = (n + 1) / 2;   /* ceil(n/2) */
    idx_t j = n / 2 + 1;     /* floor(n/2)+1, and i <= j */
    seq_t x = 1, y = 3;      /* x = c(k), y = c(k+1) */
    seq_t ci = 0, cj = 0;
    idx_t k;

    for (k = 1; ; k++) {
        seq_t z;
        if (k == i) ci = x;
        if (k == j) { cj = x; break; }
        z = 2 * y + 2 * (seq_t)(k + 1) * x;   /* c(k+2) */
        x = y;
        y = z;
    }

    if (n == 0) return cj;   /* A000898(0) * A000898(1) / 2 = 1 */
    return 2 * ci * cj;
}
