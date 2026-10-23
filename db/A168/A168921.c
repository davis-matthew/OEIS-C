/*
 * A168921  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A168921
    Number of reduced words of length n in the Coxeter group on 4 generators
    S_i with relations (S_i)^2 = (S_i S_j)^2 = I.
    Recurrence:
        a(0) = 1
        a(n) = 4 · 3^(n‑1)    for 1 ≤ n ≤ 21
        a(22) = 2·∑_{i=1}^{21} a(i) – 2
        a(n) = 2·∑_{i=1}^{21} a(n–i) – 3·a(n–22)    for n ≥ 23
*/
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *buf = NULL;
    static idx_t cap = 0;     /* allocated capacity (array length)   */
    static idx_t last = 0;    /* last index currently computed (≥0)  */

    /* initialize cache with first 23 terms (0…22) */
    if (buf == NULL) {
        cap = 23;                      /* will hold indices 0…22 */
        buf = (seq_t *)malloc(cap * sizeof(seq_t));

        /* explicit initial values (up to n = 21) */
        buf[0] = 1;
        buf[1] = 4;
        buf[2] = 12;
        buf[3] = 36;
        buf[4] = 108;
        buf[5] = 324;
        buf[6] = 972;
        buf[7] = 2916;
        buf[8] = 8748;
        buf[9] = 26244;
        buf[10] = 78732;
        buf[11] = 236196;
        buf[12] = 708588;
        buf[13] = 2125764;
        buf[14] = 6377292;
        buf[15] = 19131876;
        buf[16] = 57395628;
        buf[17] = 172186884;
        buf[18] = 516560652;
        buf[19] = 1549681956;
        buf[20] = 4649045868;
        buf[21] = 13947137604;

        /* compute a(22) by the special formula */
        seq_t sum = 0;
        for (idx_t i = 1; i <= 21; ++i) sum += buf[i];
        buf[22] = sum * 2 - 2;          /* 2·∑ a(i) – 2 */

        last = 22;                      /* we have computed up to index 22 */
    }

    /* grow the buffer if needed */
    if (n > last) {
        /* ensure the buffer can hold indices up to n */
        while (n > cap - 1) {
            cap *= 2;
            buf = (seq_t *)realloc(buf, cap * sizeof(seq_t));
        }

        /* compute missing entries from (last+1) up to n */
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t val;
            if (i == 22) {              /* this case never occurs here,
                                          the value is already stored */
                val = buf[22];
            } else {                    /* recurrence for i ≥ 23 */
                seq_t sum = 0;
                for (idx_t j = 1; j <= 21; ++j)
                    sum += buf[i - j];
                /* a(i) = 2*sum – 3*a(i-22) */
                val = sum * 2 - buf[i - 22] * 3;
            }
            buf[i] = val;
        }
        last = n;
    }

    return buf[n];
}
