/*
 * A008419  Crystal ball sequence for 9-dimensional cubic lattice.
 * https://oeis.org/A008419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n‑1) - 45*a(n‑2) + 120*a(n‑3) - 210*a(n‑4) +
 *            252*a(n‑5) - 210*a(n‑6) + 120*a(n‑7) - 45*a(n‑8) + 10*a(n‑9) -
 *            a(n‑10) Initial values: a(0)=1, a(1)=19, a(2)=181, a(3)=1159,
 *            a(4)=5641, a(5)=22363, a(6)=75517, a(7)=224143, a(8)=598417,
 *            a(9)=1462563
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 23, u64 n <= 286, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[10] = {
        1, 19, 181, 1159, 5641,
        22363, 75517, 224143, 598417, 1462563
    };
    if (n < 10) return init[n];

    seq_t a0 = init[9];   // a(n-1)
    seq_t a1 = init[8];   // a(n-2)
    seq_t a2 = init[7];   // a(n-3)
    seq_t a3 = init[6];   // a(n-4)
    seq_t a4 = init[5];   // a(n-5)
    seq_t a5 = init[4];   // a(n-6)
    seq_t a6 = init[3];   // a(n-7)
    seq_t a7 = init[2];   // a(n-8)
    seq_t a8 = init[1];   // a(n-9)
    seq_t a9 = init[0];   // a(n-10)

    for (idx_t i = 10; i <= n; ++i) {
        seq_t pos = 10*a0 + 120*a2 + 252*a4 + 120*a6 + 10*a8;
        seq_t neg = 45*a1 + 210*a3 + 210*a5 + 45*a7 + a9;
        seq_t next = pos - neg;

        /* shift window: a0 becomes next, others shift right */
        a9 = a8;
        a8 = a7;
        a7 = a6;
        a6 = a5;
        a5 = a4;
        a4 = a3;
        a3 = a2;
        a2 = a1;
        a1 = a0;
        a0 = next;
    }
    return a0;
}
