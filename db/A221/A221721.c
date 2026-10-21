/*
 * A221721  Number of n X 3 arrays of occupancy after each element stays put
 *          or moves to some horizontal, diagonal or antidiagonal neighbor.
 * https://oeis.org/A221721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 48*a(n-1) - 430*a(n-2) + 313*a(n-3) + 2310*a(n-4) -
 *            1178*a(n-5) - 878*a(n-6) + 2386*a(n-7) + 364*a(n-8) with initial
 *            values: a(1)=8, a(2)=318, a(3)=11733, a(4)=429160,
 *            a(5)=15672482, a(6)=572137641, a(7)=20884488408,
 *            a(8)=762319022830
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* below offset – any value OK */

    /* provide the first eight values */
    static const seq_t init[9] = {
        0ULL,                     /* unused 0 index */
        8ULL,
        318ULL,
        11733ULL,
        429160ULL,
        15672482ULL,
        572137641ULL,
        20884488408ULL,
        762319022830ULL
    };

    if (n <= 8) return init[n];

    /* ring buffer holding the last eight computed values,
       buf[0] = a(i-8), buf[7] = a(i-1) for current i   */
    seq_t buf[8];
    for (idx_t j = 0; j < 8; ++j) buf[j] = init[j + 1];

    for (idx_t i = 9; i <= n; ++i) {
        seq_t val = buf[7] * (seq_t)48
                    - buf[6] * (seq_t)430
                    + buf[5] * (seq_t)313
                    + buf[4] * (seq_t)2310
                    - buf[3] * (seq_t)1178
                    - buf[2] * (seq_t)878
                    + buf[1] * (seq_t)2386
                    + buf[0] * (seq_t)364;
        /* shift buffer: discard a(i-8), move others left */
        for (idx_t k = 0; k < 7; ++k) buf[k] = buf[k + 1];
        buf[7] = val;                     /* set a(i) */
    }
    return buf[7];                        /* a(n) */
}
