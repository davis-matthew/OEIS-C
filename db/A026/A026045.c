/*
 * A026045  a(n) = (d(n)-r(n))/5, where d = A026043 and r is the periodic
 *          sequence with fundamental period (0,2,3,0,0).
 * https://oeis.org/A026045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A026045.  a(n) is the unique integer such that
   a(n) = (d(n) - r(n)) / 5 where d = A026043 and r = (0,2,3,0,0) repeated.
   The sequence satisfies the 8‑term linear recurrence
      a(n) = 3·a(n‑1) – 3·a(n‑2) + a(n‑3) + a(n‑5) – 3·a(n‑6) + 3·a(n‑7) – a(n‑8)
   for n ≥ 9, with initial values
      a(1)=9, a(2)=13, a(3)=19, a(4)=28, a(5)=39,
      a(6)=53, a(7)=70, a(8)=91.  */

seq_t a(idx_t n)
{
    /* handle offset */
    if (n < 1) return (seq_t)0;

    /* initial terms */
    const seq_t init[8] = { (seq_t)9, (seq_t)13, (seq_t)19, (seq_t)28,
                            (seq_t)39, (seq_t)53, (seq_t)70, (seq_t)91 };

    if (n <= 8) return init[n-1];

    /* keep the last eight values – a1 = a(n‑8), a8 = a(n‑1) */
    seq_t a1 = init[0];
    seq_t a2 = init[1];
    seq_t a3 = init[2];
    seq_t a4 = init[3];
    seq_t a5 = init[4];
    seq_t a6 = init[5];
    seq_t a7 = init[6];
    seq_t a8 = init[7];

    for (idx_t k = 9; k <= n; ++k) {
        /* next = 3*a8 - 3*a7 + a6 + a4 - 3*a3 + 3*a2 - a1 */
        seq_t next = (seq_t)3 * a8 - (seq_t)3 * a7 + a6 + a4
                     - (seq_t)3 * a3 + (seq_t)3 * a2 - a1;

        /* shift values for next iteration */
        a1 = a2; a2 = a3; a3 = a4; a4 = a5; a5 = a6; a6 = a7; a7 = a8; a8 = next;
    }

    return a8;
}
