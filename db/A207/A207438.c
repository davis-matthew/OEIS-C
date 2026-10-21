/*
 * A207438  Number of nX4 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) + a(n-2) - 120*a(n-3) + 106*a(n-4) + 730*a(n-5)
 *            - 804*a(n-6) - 2308*a(n-7) + 2565*a(n-8) + 4000*a(n-9) -
 *            4254*a(n-10) - 3640*a(n-11) + 3811*a(n-12) + 1460*a(n-13) -
 *            1708*a(n-14) - 82*a(n-15) + 284*a(n-16) - 48*a(n-17) (n>18)
 *            Initial values: a(1)=9, a(2)=81, a(3)=323, a(4)=1360, a(...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[18] = {
        (seq_t)9ULL,      (seq_t)81ULL,      (seq_t)323ULL,      (seq_t)1360ULL,
        (seq_t)6525ULL,   (seq_t)32393ULL,   (seq_t)165626ULL,   (seq_t)855471ULL,
        (seq_t)4449491ULL,(seq_t)23211486ULL,(seq_t)121314285ULL,(seq_t)634633939ULL,
        (seq_t)3321850282ULL,(seq_t)17392770603ULL,(seq_t)91082814851ULL,
        (seq_t)477033173168ULL,(seq_t)2498544485965ULL,(seq_t)13087020633451ULL
    };

    if (n == 0) return (seq_t)0;
    if (n <= 18) return init[n-1];

    /* Buffer of the last 18 computed terms, initially the first 18 terms. */
    seq_t last[18];
    for (idx_t i = 0; i < 18; ++i) last[i] = init[i];

    for (idx_t k = 19; k <= n; ++k) {
        seq_t val = (seq_t)8   * last[17] + last[16];
        val -= (seq_t)120 * last[15];
        val += (seq_t)106 * last[14];
        val += (seq_t)730 * last[13];
        val -= (seq_t)804 * last[12];
        val -= (seq_t)2308* last[11];
        val += (seq_t)2565* last[10];
        val += (seq_t)4000* last[9];
        val -= (seq_t)4254* last[8];
        val -= (seq_t)3640* last[7];
        val += (seq_t)3811* last[6];
        val += (seq_t)1460* last[5];
        val -= (seq_t)1708* last[4];
        val -= (seq_t)82  * last[3];
        val += (seq_t)284 * last[2];
        val -= (seq_t)48  * last[1];

        /* Shift buffer and store new term. */
        for (idx_t j = 0; j < 17; ++j) last[j] = last[j+1];
        last[17] = val;
    }

    return last[17];
}
