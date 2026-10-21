/*
 * A204711  Number of (n+1)X7 0..1 arrays with the permanents of all 2X2
 *          subblocks equal and nonzero.
 * https://oeis.org/A204711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            4*a(n-1)+45*a(n-2)-126*a(n-3)-642*a(n-4)+1332*a(n-5)+3620*a(n-6)-5624*a(n-7)-6800*a(n-8)+8192*a(n-9)
 *            Initial values: a(1)=193, a(2)=1361, a(3)=8705, a(4)=58449,
 *            a(5)=382849, a(6)=2542369, a(7)=16748161, a(8)=110871041,
 *            a(9)=731709057
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* seed values */
    static const seq_t base[9] = {
        193ULL,
        1361ULL,
        8705ULL,
        58449ULL,
        382849ULL,
        2542369ULL,
        16748161ULL,
        110871041ULL,
        731709057ULL
    };

    /* 64‑bit or smaller types */
    if (sizeof(seq_t) <= 8) {
        seq_t w[9];
        for (idx_t i = 0; i < 9; ++i) w[i] = base[i];
        if (n <= 9) return w[n-1];

        for (idx_t k = 10; k <= n; ++k) {
            signed __int128 sum = 0;
            sum += (signed __int128)4    * (unsigned __int128)w[8];
            sum += (signed __int128)45   * (unsigned __int128)w[7];
            sum -= (signed __int128)126  * (unsigned __int128)w[6];
            sum -= (signed __int128)642  * (unsigned __int128)w[5];
            sum -= (signed __int128)5624 * (unsigned __int128)w[2];
            sum -= (signed __int128)6800 * (unsigned __int128)w[1];
            sum += (signed __int128)1332 * (unsigned __int128)w[4];
            sum += (signed __int128)3620 * (unsigned __int128)w[3];
            sum += (signed __int128)8192 * (unsigned __int128)w[0];
            seq_t next = (seq_t)sum;

            for (idx_t i = 0; i < 8; ++i) w[i] = w[i+1];
            w[8] = next;
        }
        return w[8];
    }
    /* larger or arbitrary‑precision types */
    else {
        seq_t w[9];
        for (idx_t i = 0; i < 9; ++i) w[i] = base[i];
        if (n <= 9) return w[n-1];

        for (idx_t k = 10; k <= n; ++k) {
            seq_t next = 0;
            next += (seq_t)4    * w[8];
            next += (seq_t)45   * w[7];
            next -= (seq_t)126  * w[6];
            next -= (seq_t)642  * w[5];
            next -= (seq_t)5624 * w[2];
            next -= (seq_t)6800 * w[1];
            next += (seq_t)1332 * w[4];
            next += (seq_t)3620 * w[3];
            next += (seq_t)8192 * w[0];

            for (idx_t i = 0; i < 8; ++i) w[i] = w[i+1];
            w[8] = next;
        }
        return w[8];
    }
}
