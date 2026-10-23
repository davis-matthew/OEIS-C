/*
 * A196742  Number of n X 4 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 2,3,1,0,4
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence of order 15 as given in OEIS. Base values
 *            a(1)…a(15) are hard‑coded; for n>15, a(n) =
 *            3*a(n-1)+26*a(n-2)+16*a(n-3)-99*a(n-4)+2*a(n-5)+81*a(n-6)
 *            -330*a(n-7)+340*a(n-8)-212*a(n-9)+124*a(n-10)+81*a(n-11)
 *            -30*a(n-12)+5*a(n-13)-3*a(n-14)-a(n-15)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t BASE[15] = {
    (seq_t)1ULL,
    (seq_t)7ULL,
    (seq_t)71ULL,
    (seq_t)401ULL,
    (seq_t)2923ULL,
    (seq_t)19567ULL,
    (seq_t)134507ULL,
    (seq_t)919377ULL,
    (seq_t)6283897ULL,
    (seq_t)42987489ULL,
    (seq_t)293903925ULL,
    (seq_t)2009922151ULL,
    (seq_t)13744157339ULL,
    (seq_t)93985887721ULL,
    (seq_t)642701573055ULL
};

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)0;           /* safety for calls with n = 0 */
    }
    if (n <= 15) {
        return BASE[n - 1];
    }

    seq_t last[15];
    for (int i = 0; i < 15; ++i) {
        last[i] = BASE[i];
    }
    seq_t cur = 0;

    for (idx_t k = 16; k <= n; ++k) {
        cur = (seq_t)3   * last[14] + (seq_t)26  * last[13]
           + (seq_t)16  * last[12] - (seq_t)99  * last[11]
           + (seq_t)2   * last[10] + (seq_t)81  * last[9]
           - (seq_t)330 * last[8]  + (seq_t)340 * last[7]
           - (seq_t)212 * last[6]  + (seq_t)124 * last[5]
           + (seq_t)81  * last[4]  - (seq_t)30  * last[3]
           + (seq_t)5   * last[2]  - (seq_t)3   * last[1]
           - last[0];

        for (int i = 0; i < 14; ++i) {
            last[i] = last[i + 1];
        }
        last[14] = cur;
    }

    return cur;
}
