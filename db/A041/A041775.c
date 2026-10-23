/*
 * A041775  Denominators of continued fraction convergents to sqrt(408).
 * https://oeis.org/A041775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = 202*a(n-2) - a(n-4),  n > 3
   Initial values: a(0)=1, a(1)=5, a(2)=201, a(3)=1010 */
seq_t a(idx_t n)
{
    /* Handle base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)201;
    if (n == 3) return (seq_t)1010;

    /* Rolling buffer for the last four terms */
    seq_t a0 = (seq_t)1;       /* a(n-4) */
    seq_t a1 = (seq_t)5;       /* a(n-3) */
    seq_t a2 = (seq_t)201;     /* a(n-2) */
    seq_t a3 = (seq_t)1010;    /* a(n-1) */
    seq_t next;

    /* Compute up to a(n) */
    for (idx_t i = 4; i <= n; ++i) {
        next = (seq_t)202 * a2 - a0; /* a(i) = 202*a(i-2) - a(i-4) */
        a0 = a1;    /* shift window forward */
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
