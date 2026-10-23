/*
 * A172210  Number of ways to place 5 nonattacking bishops on a 5 X n board.
 * https://oeis.org/A172210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (625n^5-11250n^4+98875n^3-515250n^2+1566016n-2194944)/24
 *            for n≥16; * for n<16 the value is computed by an exhaustive
 *            search
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 47, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    if (n <= 15) {
        static seq_t cache[16] = {0};
        if (cache[n])
            return cache[n];

        idx_t tot = 5 * n;
        seq_t result = 0;

        for (idx_t i = 0; i + 4 < tot; ++i) {
            idx_t r1 = i / n, c1 = i % n;
            for (idx_t j = i + 1; j + 3 < tot; ++j) {
                idx_t r2 = j / n, c2 = j % n;
                for (idx_t k = j + 1; k + 2 < tot; ++k) {
                    idx_t r3 = k / n, c3 = k % n;
                    for (idx_t l = k + 1; l + 1 < tot; ++l) {
                        idx_t r4 = l / n, c4 = l % n;
                        for (idx_t m = l + 1; m < tot; ++m) {
                            idx_t r5 = m / n, c5 = m % n;

                            int md0 = (int)r1 - (int)c1;
                            int md1 = (int)r2 - (int)c2;
                            int md2 = (int)r3 - (int)c3;
                            int md3 = (int)r4 - (int)c4;
                            int md4 = (int)r5 - (int)c5;

                            if (md0==md1||md0==md2||md0==md3||md0==md4||
                                md1==md2||md1==md3||md1==md4||
                                md2==md3||md2==md4||
                                md3==md4) continue;

                            int ad0 = (int)r1 + (int)c1;
                            int ad1 = (int)r2 + (int)c2;
                            int ad2 = (int)r3 + (int)c3;
                            int ad3 = (int)r4 + (int)c4;
                            int ad4 = (int)r5 + (int)c5;

                            if (ad0==ad1||ad0==ad2||ad0==ad3||ad0==ad4||
                                ad1==ad2||ad1==ad3||ad1==ad4||
                                ad2==ad3||ad2==ad4||
                                ad3==ad4) continue;

                            ++result;
                        }
                    }
                }
            }
        }

        cache[n] = result;
        return result;
    }

    /* n ≥ 16 */
    __int128 num =
        (__int128)625 * n * n * n * n * n
        - (__int128)11250 * n * n * n * n
        + (__int128)98875 * n * n * n
        - (__int128)515250 * n * n
        + (__int128)1566016 * n
        - (__int128)2194944;
    return (seq_t)(num / 24);
}
