/*
 * A023952  Expansion of g.f. 1/((1-x)*(1-6*x)*(1-7*x)*(1-11*x)).
 * https://oeis.org/A023952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 25*a(n-1) - 209*a(n-2) + 647*a(n-3) - 462*a(n-4);
   Initial values: a(0)=1, a(1)=25, a(2)=416, a(3)=5822 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)25;
    if (n == 2) return (seq_t)416;
    if (n == 3) return (seq_t)5822;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)25;
    seq_t a2 = (seq_t)416;
    seq_t a3 = (seq_t)5822;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = a3 * (seq_t)25;
        next -= a2 * (seq_t)209;
        next += a1 * (seq_t)647;
        next -= a0 * (seq_t)462;

        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
