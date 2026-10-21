/*
 * A200083  Number of 0..n arrays x(0..4) of 5 elements with zero 3rd
 *          differences.
 * https://oeis.org/A200083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            a(n-1)+a(n-3)-a(n-5)+a(n-6)-2*a(n-7)+a(n-8)-a(n-9)+a(n-11)+a(n-13)-a(n-14);
 *            Initial values: a(1)=2, a(2)=3, a(3)=8, a(4)=17, a(5)=26,
 *            a(6)=43, a(7)=64, a(8)=89, a(9)=122, a(10)=163, a(11)=208,
 *            a(12)=269, a(13)=334, a(14)=407
 * Range:     u8 n <= 11, u16 n <= 80, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t init[15] = {
        0,  /* unused 0th element */
        2,  /* a(1) */
        3,  /* a(2) */
        8,  /* a(3) */
        17, /* a(4) */
        26, /* a(5) */
        43, /* a(6) */
        64, /* a(7) */
        89, /* a(8) */
        122,/* a(9) */
        163,/* a(10) */
        208,/* a(11) */
        269,/* a(12) */
        334,/* a(13) */
        407 /* a(14) */
    };

    if (n <= 14) {
        return init[n];
    }

    /* Use a circular buffer of size 15 to store the last values a(i-14) .. a(i-1). */
    seq_t buf[15];
    /* Initialize buffer with the known values a(1) .. a(14). */
    for (idx_t i = 1; i <= 14; ++i) {
        buf[i % 15] = init[i];
    }

    seq_t cur;
    for (idx_t i = 15; i <= n; ++i) {
        cur = buf[(i - 1) % 15]
            + buf[(i - 3) % 15]
            - buf[(i - 5) % 15]
            + buf[(i - 6) % 15]
            - (seq_t)2 * buf[(i - 7) % 15]
            + buf[(i - 8) % 15]
            - buf[(i - 9) % 15]
            + buf[(i - 11) % 15]
            + buf[(i - 13) % 15]
            - buf[(i - 14) % 15];
        buf[i % 15] = cur;
    }

    return buf[n % 15];
}
