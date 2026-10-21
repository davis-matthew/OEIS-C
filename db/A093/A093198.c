/*
 * A093198  Number of 4 X 4 symmetric magic squares with line sum 2n.
 * https://oeis.org/A093198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 286, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1,  a(1) = 11,  a(2) = 53,  a(3) = 164,  a(4) = 397,  a(5) = 819;
   for n ≥ 6:
     a(n) = 4*a(n-1) - 5*a(n-2) + 5*a(n-4) - 4*a(n-5) + a(n-6). */

seq_t a(idx_t n) {
    /* initial six terms */
    static const seq_t init[6] = {
        (seq_t)1,
        (seq_t)11,
        (seq_t)53,
        (seq_t)164,
        (seq_t)397,
        (seq_t)819
    };

    if (n < 6)
        return init[n];

    seq_t a0 = init[0], a1 = init[1], a2 = init[2],
          a3 = init[3], a4 = init[4], a5 = init[5];

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = 4 * a5 - 5 * a4 + 5 * a2 - 4 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }
    return a5;
}
