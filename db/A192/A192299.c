/*
 * A192299  0-sequence of reduction of (n^2+n+1) by x^2 -> x+1.
 * https://oeis.org/A192299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence
 *            a(n)=4*a(n‑1)−3*a(n‑2)−5*a(n‑3)+5*a(n‑4)+3*a(n‑5)−2*a(n‑6)−a(n‑7).
 *            Initial values (offset 1): a(1)=1, a(2)=1, a(3)=8, a(4)=21,
 *            a(5)=63, a(6)=156, a(7)=371
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t base[8] = {0, 1, 1, 8, 21, 63, 156, 371};

    if (n <= 7)
        return base[n];

    seq_t a1 = base[1], a2 = base[2], a3 = base[3],
          a4 = base[4], a5 = base[5], a6 = base[6], a7 = base[7];

    for (idx_t i = 8; i <= n; ++i) {
        /* compute a(i) = 4*a7 + 5*a4 + 3*a3  - (3*a6 + 5*a5 + 2*a2 + a1) */
        seq_t left  = 4 * a7 + 5 * a4 + 3 * a3;
        seq_t right = 3 * a6 + 5 * a5 + 2 * a2 + a1;
        seq_t ai    = left - right;

        /* shift window */
        a1 = a2; a2 = a3; a3 = a4;
        a4 = a5; a5 = a6; a6 = a7;
        a7 = ai;
    }

    return a7;
}
