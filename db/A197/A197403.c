/*
 * A197403  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 3,0,1,1,1
 *          for x=0,1,2,3,4.
 * https://oeis.org/A197403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) + 7*a(n-3) - 10*a(n-4) - 39*a(n-5) -
 *            30*a(n-6) - 12*a(n-7) + 5*a(n-8) + 2*a(n-9) Initial values:
 *            a(1)=3, a(2)=9, a(3)=20, a(4)=71, a(5)=227, a(6)=664, a(7)=2107,
 *            a(8)=6675, a(9)=20696. This recurrence holds for n >= 10
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If the offset is used undefine, else normal */
    if (n == 0) return 0;

    /* Initial 9 terms */
    static const seq_t init[9] = {
        (seq_t)3,
        (seq_t)9,
        (seq_t)20,
        (seq_t)71,
        (seq_t)227,
        (seq_t)664,
        (seq_t)2107,
        (seq_t)6675,
        (seq_t)20696
    };

    if (n <= 9) return init[n-1];

    /* Keep the last 9 values: last[0] = a(n-9), ..., last[8] = a(n-1) */
    seq_t last[9];
    for (int i = 0; i < 9; ++i) last[i] = init[i];

    for (idx_t i = 10; i <= n; ++i) {
        /* Positive part: 2*a(n-1) + 4*a(n-2) + 7*a(n-3) + 5*a(n-8) + 2*a(n-9) */
        seq_t pos = (seq_t)2 * last[8]   /* 2*a(n-1) */
                 + (seq_t)4 * last[7]   /* 4*a(n-2) */
                 + (seq_t)7 * last[6]   /* 7*a(n-3) */
                 + (seq_t)5 * last[1]   /* 5*a(n-8) */
                 + (seq_t)2 * last[0];  /* 2*a(n-9) */

        /* Negative part: -10*a(n-4) -39*a(n-5) -30*a(n-6) -12*a(n-7) */
        seq_t neg = (seq_t)10 * last[5]  /* 10*a(n-4) */
                 + (seq_t)39 * last[4]  /* 39*a(n-5) */
                 + (seq_t)30 * last[3]  /* 30*a(n-6) */
                 + (seq_t)12 * last[2]; /* 12*a(n-7) */

        seq_t next = pos - neg;  /* a(n) */

        /* Shift: discard a(n-9), slide others, append a(n) */
        for (int j = 0; j < 8; ++j) last[j] = last[j+1];
        last[8] = next;
    }
    return last[8];  /* a(n) */
}
