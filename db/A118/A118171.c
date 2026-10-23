/*
 * A118171  Decimal representation of n-th iteration of the Rule 158
 *          elementary cellular automaton starting with a single black cell.
 * https://oeis.org/A118171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (1/30) * (-16 + (-4)^n - 10*(-1)^n + 55*4^n) Recurrence:
 *            a(0)=1, a(1)=7, a(2)=29, a(3)=115, a(n) = 17*a(n-2) - 16*a(n-4)
 *            for n > 3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Direct values for the first four indices */
    const seq_t init[4] = {(seq_t)1, (seq_t)7, (seq_t)29, (seq_t)115};

    if (n < 4)            /* Fast path for small n */
        return init[n];

    /* Iterate using the 4‑term recurrence.
       t0 .. t3 represent a(n‑4), a(n‑3), a(n‑2), a(n‑1) respectively. */
    seq_t t0 = init[0];
    seq_t t1 = init[1];
    seq_t t2 = init[2];
    seq_t t3 = init[3];

    idx_t i;
    for (i = 4; i <= n; ++i) {
        seq_t next = (seq_t)17 * t2 - (seq_t)16 * t0;
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = next;
    }
    return t3;
}
