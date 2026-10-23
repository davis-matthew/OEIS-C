/*
 * A020573  Expansion of g.f. 1/((1 - 6*x)*(1 - 7*x)*(1 - 11*x)).
 * https://oeis.org/A020573
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

/* Sequence A020573: Expansion of g.f. 1/((1 - 6*x)*(1 - 7*x)*(1 - 11*x)).
   Recurrence: a(0)=1, a(1)=24, a(2)=391, a(n)=24*a(n-1)-185*a(n-2)+462*a(n-3) for n>=3. */

extern seq_t a(idx_t n) {
    /* Base cases */
    if (n == 0) return 1;
    if (n == 1) return 24;
    if (n == 2) return 391;

    seq_t a0 = 1;   /* a(n-3) */
    seq_t a1 = 24;  /* a(n-2) */
    seq_t a2 = 391; /* a(n-1) */
    seq_t an = 0;   /* a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        an = 24 * a2 - 185 * a1 + 462 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}
