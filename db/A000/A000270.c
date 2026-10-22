/*
 * A000270  For n >= 2, a(n) = b(n+1)+b(n)+b(n-1), where the b(i) are the
 *          ménage numbers A000179; a(0)=a(1)=1.
 * https://oeis.org/A000270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = b(n+1) + b(n) + b(n-1) for n >= 2, a(0) = a(1) = 1, where
 *            b = A000179 (menage numbers) satisfies b(k) = k*b(k-1) +
 *            (k*b(k-2) - 4*(-1)^k)/(k-2), b(2) = 0, b(3) = 1, b(4) = 2 (this
 *            is the standard recurrence (k-2)*b(k) = k*(k-2)*b(k-1) +
 *            k*b(k-2) - 4*(-1)^k with the exact division split off...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)1;
    if (n == 2) return (seq_t)0;

    seq_t x = 0;   /* b(k-2) */
    seq_t y = 1;   /* b(k-1) */
    seq_t z = 2;   /* b(k)   */
    idx_t k = 4;

    while (k < n + 1) {
        k++;
        seq_t t = (seq_t)k * y;
        seq_t q;
        if ((k & 1) == 0)
            q = (t - 4) / (seq_t)(k - 2);
        else
            q = (t + 4) / (seq_t)(k - 2);
        seq_t nb = (seq_t)k * z + q;
        x = y;
        y = z;
        z = nb;
    }
    /* now z = b(n+1), y = b(n), x = b(n-1) */
    return z + y + x;
}
