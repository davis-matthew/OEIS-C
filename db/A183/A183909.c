/*
 * A183909  Number of nondecreasing arrangements of n+2 numbers in 0..6 with
 *          each number being the sum mod 7 of two others.
 * https://oeis.org/A183909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) * +
 *            21*a(n-5) - 7*a(n-6) + a(n-7) for n > 10 * Initial values: *
 *            a(1)=1, a(2)=1, a(3)=22, a(4)=136, a(5)=470, * a(6)=1193,
 *            a(7)=2525, a(8)=4752, a(9)=8238, a(10)=13438
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined, but safe */
    /* initial values up to n = 10 */
    if (n <= 10) {
        switch (n) {
            case 1: return (seq_t)1;
            case 2: return (seq_t)1;
            case 3: return (seq_t)22;
            case 4: return (seq_t)136;
            case 5: return (seq_t)470;
            case 6: return (seq_t)1193;
            case 7: return (seq_t)2525;
            case 8: return (seq_t)4752;
            case 9: return (seq_t)8238;
            case 10: return (seq_t)13438;
        }
    }
    /* recurrence for n > 10 */
    seq_t ring[7];
    /* ring[0] = a(i-7), ring[6] = a(i-1) at start, i = 11 */
    ring[0] = (seq_t)136;   /* a(4) */
    ring[1] = (seq_t)470;   /* a(5) */
    ring[2] = (seq_t)1193;  /* a(6) */
    ring[3] = (seq_t)2525;  /* a(7) */
    ring[4] = (seq_t)4752;  /* a(8) */
    ring[5] = (seq_t)8238;  /* a(9) */
    ring[6] = (seq_t)13438; /* a(10) */

    seq_t val = (seq_t)0;
    for (idx_t i = 11; i <= n; ++i) {
        val = ring[6] * (seq_t)7
            - ring[5] * (seq_t)21
            + ring[4] * (seq_t)35
            - ring[3] * (seq_t)35
            + ring[2] * (seq_t)21
            - ring[1] * (seq_t)7
            + ring[0];

        /* shift ring leftwards */
        ring[0] = ring[1];
        ring[1] = ring[2];
        ring[2] = ring[3];
        ring[3] = ring[4];
        ring[4] = ring[5];
        ring[5] = ring[6];
        ring[6] = val;          /* a(i) becomes the new a(i-1) for next step */
    }
    return val;                  /* a(n) */
}
