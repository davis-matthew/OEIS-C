/*
 * A156923  Fourth right hand column (n-m=3) of the A156920 triangle.
 * https://oeis.org/A156923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 30*a(n‑1) - 385*a(n‑2) + 2776*a(n‑3) - 12418*a(n‑4) * +
 *            35908*a(n‑5) - 67818*a(n‑6) + 82552*a(n‑7) - * 62109*a(n‑8) +
 *            26190*a(n‑9) - 4725*a(n‑10) * with a(0)=1, a(1)=37, a(2)=646,
 *            a(3)=8030, a(4)=82610, * a(5)=756218, a(6)=6411720,
 *            a(7)=51586344, a(8)=400011435, * a(9)=3020658295
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* seed values */
    static const seq_t init[10] = {
        (seq_t)1ULL,
        (seq_t)37ULL,
        (seq_t)646ULL,
        (seq_t)8030ULL,
        (seq_t)82610ULL,
        (seq_t)756218ULL,
        (seq_t)6411720ULL,
        (seq_t)51586344ULL,
        (seq_t)400011435ULL,
        (seq_t)3020658295ULL
    };

    /* for n < 10 return the predetermined value */
    if (n < 10u)
        return init[(idx_t)n];

    /* buf holds the last 10 computed values: buf[0] = a(k‑9), ..., buf[9] = a(k‑1) */
    seq_t buf[10];
    for (idx_t i = 0; i < 10; ++i)
        buf[i] = init[i];

    /* iterate from 10 up through n */
    for (idx_t k = 10; k <= n; ++k) {
        seq_t s = (seq_t)30   * buf[9];
        s -= (seq_t)385   * buf[8];
        s += (seq_t)2776  * buf[7];
        s -= (seq_t)12418 * buf[6];
        s += (seq_t)35908 * buf[5];
        s -= (seq_t)67818 * buf[4];
        s += (seq_t)82552 * buf[3];
        s -= (seq_t)62109 * buf[2];
        s += (seq_t)26190 * buf[1];
        s -= (seq_t)4725  * buf[0];

        /* shift the buffer left by one position */
        for (idx_t j = 0; j < 9; ++j)
            buf[j] = buf[j + 1];
        buf[9] = s;          /* new value becomes a(k) */
    }

    return buf[9];           /* a(n) is stored in buf[9] after the loop */
}
