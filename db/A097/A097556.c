/*
 * A097556  Number of positive words of length n in the monoid Br_9 of
 *          positive braids on 10 strands.
 * https://oeis.org/A097556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9 a(n-1) - 32 a(n-2) + 63 a(n-3) - 84 a(n-4) + 81 a(n-5)
 *            - 56 a(n-6) + 27 a(n-7) - 8 a(n-8) + a(n-9) (n ≥ 15) with
 *            initial values a(0)…a(14) given below
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 33, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first 15 terms of the sequence */
    static const seq_t init[15] = {
        1ULL, 9ULL, 56ULL, 279ULL, 1223ULL, 4932ULL,
        18833ULL, 69345ULL, 249166ULL, 880525ULL,
        3076295ULL, 10662459ULL, 36749785ULL, 126161246ULL,
        431880044ULL
    };

    if (n <= 14)
        return init[n];

    /* state holding a[n-9] … a[n-1] */
    seq_t last[9];
    for (int i = 0; i < 9; ++i)
        last[i] = init[6 + i];   /* last[0]=a[6] , … , last[8]=a[14] */

    seq_t val;
    for (idx_t i = 15; i <= n; ++i) {
        val =  (seq_t)9   * last[8]
             - (seq_t)32  * last[7]
             + (seq_t)63  * last[6]
             - (seq_t)84  * last[5]
             + (seq_t)81  * last[4]
             - (seq_t)56  * last[3]
             + (seq_t)27  * last[2]
             - (seq_t)8   * last[1]
             +            last[0];

        /* shift window */
        for (int j = 0; j < 8; ++j)
            last[j] = last[j + 1];
        last[8] = val;
    }
    return last[8];
}
