/*
 * A020713  Pisot sequences E(5,9), P(5,9).
 * https://oeis.org/A020713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3); a(0) = 5, a(1) = 9, a(2) = 16
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 36, u64 n <= 75, u128 n <= 154,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)5;
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)16;

    /* t0 = a(n-3), t1 = a(n-2), t2 = a(n-1) */
    seq_t t0 = (seq_t)5;
    seq_t t1 = (seq_t)9;
    seq_t t2 = (seq_t)16;
    seq_t next;

    for (idx_t i = 3; i <= n; ++i) {
        next = ((seq_t)2 * t2) - t1 + t0;
        t0 = t1;
        t1 = t2;
        t2 = next;
    }

    return t2;
}
