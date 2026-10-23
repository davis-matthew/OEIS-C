/*
 * A189276  Number of nX5 array permutations with each element not moved or
 *          moved diagonally or antidiagonally by one.
 * https://oeis.org/A189276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 25*a(n-1) - 1067*a(n-3) + 1111*a(n-4) + 19248*a(n-5) -
 *            37086*a(n-6) + 40374*a(n-7) - 18000*a(n-8) - 44874*a(n-9) +
 *            111258*a(n-10) - 91872*a(n-11) + 9999*a(n-12) - 13743*a(n-13) -
 *            891*a(n-15) - 81*a(n-16) Base values (OEIS A189276): a(1) = 1
 *            a(2) = 64 a(3) = 1125 a(4) = 29241 a(5) = 670480...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    static bool init_done = false;
    static seq_t init[17];   /* init[0] unused, init[1]..init[16] hold terms */

    if (!init_done) {
        init[0] = 0;
        init[1] = seq_from_str("1");
        init[2] = seq_from_str("64");
        init[3] = seq_from_str("1125");
        init[4] = seq_from_str("29241");
        init[5] = seq_from_str("670480");
        init[6] = seq_from_str("15594601");
        init[7] = seq_from_str("361199937");
        init[8] = seq_from_str("8366395024");
        init[9] = seq_from_str("193788877917");
        init[10] = seq_from_str("4488512569321");
        init[11] = seq_from_str("103963620831296");
        init[12] = seq_from_str("2408013388109169");
        init[13] = seq_from_str("55774648500091873");
        init[14] = seq_from_str("1291857814182989824");
        init[15] = seq_from_str("29922137343175603701");
        init[16] = seq_from_str("693059476202859612025");
        init_done = true;
    }

    if (n <= 16)
        return init[(idx_t)n];

    /* Sliding window of the last 16 terms: buf[0] = a(n-16), ..., buf[15] = a(n-1) */
    seq_t buf[16];
    for (int i = 0; i < 16; ++i)
        buf[i] = init[i + 1];

    for (idx_t i = 17; i <= n; ++i) {
        seq_t pos = buf[15] * (seq_t)25;
        pos += buf[12] * (seq_t)1111;
        pos += buf[11] * (seq_t)19248;
        pos += buf[9]  * (seq_t)40374;
        pos += buf[6]  * (seq_t)111258;
        pos += buf[4]  * (seq_t)9999;

        seq_t neg = buf[13] * (seq_t)1067;
        neg += buf[10] * (seq_t)37086;
        neg += buf[8]  * (seq_t)18000;
        neg += buf[7]  * (seq_t)44874;
        neg += buf[5]  * (seq_t)91872;
        neg += buf[3]  * (seq_t)13743;
        neg += buf[1]  * (seq_t)891;
        neg += buf[0]  * (seq_t)81;

        seq_t cur = pos;
        cur -= neg;          /* pos ≥ neg for all n ≥ 17, so no underflow */

        /* shift buffer: drop oldest term, insert new one */
        for (int j = 0; j < 15; ++j)
            buf[j] = buf[j + 1];
        buf[15] = cur;
    }

    return buf[15];  /* a(n) */
}
