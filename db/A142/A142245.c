/*
 * A142245  Expansion of 2*x*(6 + 5*x) / ((1 - x)*(1 - x - x^2)).
 * https://oeis.org/A142245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=12, a(2)=34, a(n)=2*a(n-1)-a(n-3) for n>=3 */
seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 12;
    if (n == 2) return 34;

    seq_t a0 = 0;      /* a(n-3) */
    seq_t a1 = 12;     /* a(n-2) */
    seq_t a2 = 34;     /* a(n-1) */
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        an = a2 + a2;   /* 2*a(n-1) */
        an = an - a0;   /* 2*a(n-1) - a(n-3) */
        a0 = a1;        /* shift window */
        a1 = a2;
        a2 = an;
    }
    return a2;
}
