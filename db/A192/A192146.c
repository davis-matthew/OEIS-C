/*
 * A192146  1-sequence of reduction of pentagonal numbers sequence by x^2 ->
 *          x+1.
 * https://oeis.org/A192146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) - 5*a(n-3) + 5*a(n-4) + 3*a(n-5) -
 *            2*a(n-6) - a(n-7) (n > 7) Initial values: a(1)=0, a(2)=5,
 *            a(3)=17, a(4)=61, a(5)=166, a(6)=421, a(7)=981
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values indexed from 1; base[0] unused */
    static const seq_t base[8] = {
        (seq_t)0, /* base[0] unused */
        (seq_t)0, /* a(1) */
        (seq_t)5, /* a(2) */
        (seq_t)17,/* a(3) */
        (seq_t)61,/* a(4) */
        (seq_t)166/* a(5) */
    };
    /* Extend base array to include a(6) and a(7) for clarity */
    static const seq_t base6 = (seq_t)421;
    static const seq_t base7 = (seq_t)981;

    /* If n is outside the defined range, return 0. */
    if (n < 1)
        return (seq_t)0;

    /* Handle the first seven terms directly. */
    if (n <= 7) {
        switch (n) {
            case 1: return base[1];
            case 2: return base[2];
            case 3: return base[3];
            case 4: return base[4];
            case 5: return base[5];
            case 6: return base6;
            case 7: return base7;
        }
    }

    /* Compute iteratively for n > 7 using the 7‑term recurrence. */
    seq_t last7[7];
    last7[0] = base7;  /* a(n-1) for n=8 -> a(7) */
    last7[1] = base6;  /* a(6)          */
    last7[2] = base[5];/* a(5)          */
    last7[3] = base[4];/* a(4)          */
    last7[4] = base[3];/* a(3)          */
    last7[5] = base[2];/* a(2)          */
    last7[6] = base[1];/* a(1)          */
    seq_t val = 0;     /* placeholder */

    for (idx_t i = 8; i <= n; ++i) {
        val = 4 * last7[0]
              - 3 * last7[1]
              - 5 * last7[2]
              + 5 * last7[3]
              + 3 * last7[4]
              - 2 * last7[5]
              - last7[6];

        /* Shift the window for the next iteration */
        last7[6] = last7[5];
        last7[5] = last7[4];
        last7[4] = last7[3];
        last7[3] = last7[2];
        last7[2] = last7[1];
        last7[1] = last7[0];
        last7[0] = val;
    }
    return val;
}
