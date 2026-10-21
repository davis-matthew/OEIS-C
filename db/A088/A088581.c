/*
 * A088581  a(n) = n*x^n + (n-1)*x^(n-1) + . . . + x + 1 for x=3.
 * https://oeis.org/A088581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((6*n-3)*3^n + 7)/4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 36, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Uses recurrence: a(n) = 6*a(n-1)-8*a(n-2)-6*a(n-3)+9*a(n-4) for n>3) */

seq_t a(idx_t n)
{
    /* first four terms are known */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)22;
    if (n == 3) return (seq_t)103;

    /* a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)4;   /* a(1) */
    seq_t a2 = (seq_t)22;  /* a(2) */
    seq_t a3 = (seq_t)103; /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)6 * a3;           /* 6*a(n-1) */
        next -= (seq_t)8 * a2;               /* -8*a(n-2) */
        next -= (seq_t)6 * a1;               /* -6*a(n-3) */
        next += (seq_t)9 * a0;               /* +9*a(n-4) */

        /* shift the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
