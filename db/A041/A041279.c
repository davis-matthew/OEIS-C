/*
 * A041279  Denominators of continued fraction convergents to sqrt(152).
 * https://oeis.org/A041279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A041279 – Denominators of continued fraction convergents to sqrt(152)
 *
 * Recurrence: a(n) = 74*a(n-2) - a(n-4)
 * Initial terms:
 *   a(0) = 1,  a(1) = 3,  a(2) = 73,  a(3) = 222
 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)73;
    if (n == 3) return (seq_t)222;

    seq_t a0 = (seq_t)1;   /* a(n-4) */
    seq_t a1 = (seq_t)3;   /* a(n-3) */
    seq_t a2 = (seq_t)73;  /* a(n-2) */
    seq_t a3 = (seq_t)222; /* a(n-1) */
    seq_t an = 0;

    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)74 * a2 - a0; /* a(i) = 74*a(i-2) - a(i-4) */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
