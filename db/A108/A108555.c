/*
 * A108555  Antidiagonal sums of square array A108553, in which row n equals
 *          the crystal ball sequence for D_n lattice.
 * https://oeis.org/A108555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 28*a(n-2) + 32*a(n-3) - 2*a(n-4) - 10*a(n-5) -
 *            4*a(n-6) + 3*a(n-8) + a(n-9) with initial values
 *            a(0)=1,a(1)=2,a(2)=5,a(3)=12,a(4)=35,a(5)=116,a(6)=411,a(7)=1482,a(8)=5333
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial 9 terms of the sequence */
    const seq_t init[9] = { (seq_t)1, (seq_t)2, (seq_t)5, (seq_t)12,
                            (seq_t)35, (seq_t)116, (seq_t)411, (seq_t)1482,
                            (seq_t)5333 };

    if (n < 9) return init[n];

    /* Buffer holding the last 9 values a(n-9) .. a(n-1) */
    seq_t buf[9];
    for (int i = 0; i < 9; ++i) buf[i] = init[i];

    for (idx_t i = 9; i <= n; ++i) {
        seq_t next = 9 * buf[8];
        next += 32 * buf[6];
        next += 3 * buf[1];
        next += buf[0];
        next -= 28 * buf[7];
        next -= 2 * buf[5];
        next -= 10 * buf[4];
        next -= 4 * buf[3];

        /* shift buffer left, discarding a(n-9) and inserting a(n) */
        for (int j = 0; j < 8; ++j) buf[j] = buf[j + 1];
        buf[8] = next;
    }
    return buf[8];
}
