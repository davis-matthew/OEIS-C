/*
 * A142965  One fourth of third column (m=2) of triangle A142963.
 * https://oeis.org/A142965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 40*a(n-2) + 82*a(n-3) - 91*a(n-4) + 52*a(n-5)
 *            - 12*a(n-6) Initial values: a(0)=1, a(1)=18, a(2)=129, a(3)=646,
 *            a(4)=2685, a(5)=10002
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 36, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[6] = {1, 18, 129, 646, 2685, 10002};

    if (n < 6) {
        return init[n];
    }

    seq_t a0 = init[0];
    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];
    seq_t a4 = init[4];
    seq_t a5 = init[5];

    for (idx_t i = 6; i <= n; ++i) {
        seq_t small = 10 * a5;
        small = small + 82 * a3;
        small = small + 52 * a1;

        seq_t large = 40 * a4;
        large = large + 91 * a2;
        large = large + 12 * a0;

        seq_t next = small - large;

        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }

    return a5;
}
