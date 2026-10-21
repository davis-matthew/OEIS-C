/*
 * A092439  Sequence arising from enumeration of domino tilings of Aztec
 *          Pillow-like regions.
 * https://oeis.org/A092439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9*a(n-1) - 30*a(n-2) + 42*a(n-3) - 9*a(n-4) - 39*a(n-5) +
 *            40*a(n-6) - 12*a(n-7); Initial values: a(0)=0, a(1)=0, a(2)=6,
 *            a(3)=30, a(4)=140, a(5)=560, a(6)=2058. The recurrence is
 *            calculated using only addition and subtraction of positive
 *            intermediate results to avoid wrap‑around on unsigne...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 6;
    if (n == 3) return 30;
    if (n == 4) return 140;
    if (n == 5) return 560;
    if (n == 6) return 2058;

    seq_t a0 = 0;          // a(n-7)
    seq_t a1 = 0;          // a(n-6)
    seq_t a2 = 6;          // a(n-5)
    seq_t a3 = 30;         // a(n-4)
    seq_t a4 = 140;        // a(n-3)
    seq_t a5 = 560;        // a(n-2)
    seq_t a6 = 2058;       // a(n-1)
    seq_t an = 0;

    for (idx_t i = 7; i <= n; ++i) {
        seq_t pos = a6 * (seq_t)9 + a4 * (seq_t)42 + a1 * (seq_t)40;
        seq_t neg = a5 * (seq_t)30 + a3 * (seq_t)9 + a2 * (seq_t)39 + a0 * (seq_t)12;
        an = pos - neg;                 // guaranteed non‑negative

        /* shift window for next term */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = an;
    }

    return an;
}
