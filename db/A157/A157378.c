/*
 * A157378  a(n) = 43046721*n^2 - 50729652*n + 14945957.
 * https://oeis.org/A157378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 43046721*n^2 - 50729652*n + 14945957
 * Range:     u8 none, u16 none, u32 n <= 10, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3);
   Initial values: a(1)=7263026, a(2)=85673537, a(3)=250177490 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)7263026;
    if (n == 2) return (seq_t)85673537;
    if (n == 3) return (seq_t)250177490;

    seq_t a1 = (seq_t)7263026;
    seq_t a2 = (seq_t)85673537;
    seq_t a3 = (seq_t)250177490;
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        next = ((seq_t)3) * a3 - ((seq_t)3) * a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
