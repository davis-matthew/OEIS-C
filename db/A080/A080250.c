/*
 * A080250  Expansion of 1/((1-x)(1-4x)(1-10x)(1-20x)).
 * https://oeis.org/A080250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1350*20^n - 950*10^n + 114*4^n - 1) / 513 Recurrence:
 *            a(n) = 35*a(n-1) - 354*a(n-2) + 1120*a(n-3) - 800*a(n-4) Initial
 *            values: a(0)=1, a(1)=35, a(2)=871, a(3)=19215
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)35;
    if (n == 2) return (seq_t)871;
    if (n == 3) return (seq_t)19215;

    /* Iteratively apply the linear recurrence */
    seq_t a0 = (seq_t)1;      /* a(n-4) */
    seq_t a1 = (seq_t)35;     /* a(n-3) */
    seq_t a2 = (seq_t)871;    /* a(n-2) */
    seq_t a3 = (seq_t)19215;  /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t a4 = 35 * a3 - 354 * a2 + 1120 * a1 - 800 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }
    return a3; /* a(n) */
}
