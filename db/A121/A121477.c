/*
 * A121477  Zeroless palindromes with digit sum 9.
 * https://oeis.org/A121477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Zeroless palindromes with digit sum 9.
   The sequence is finite: only palindromes of odd length up to 9
   can have digit sum 9 without using the digit 0.  The function
   generates all such palindromes, sorts them, and returns the
   n‑th smallest value (1‑based). */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t pal[32];
    static size_t pal_cnt = 0;
    static bool initialized = false;
    if (!initialized) {
        pal_cnt = 0;
        /* length 1: 9 */
        pal[pal_cnt++] = 9;

        /* length 3: k = 1 */
        for (int m = 1; m <= 9; m += 2) {
            int diff = 9 - m;
            if ((diff & 1) != 0) continue;          /* t must be integer */
            int t = diff / 2;
            if (t < 1 || t > 9) continue;           /* digit 1‑9 */
            int d1 = t;
            seq_t val = 0;
            val = val * 10 + d1;
            val = val * 10 + m;
            val = val * 10 + d1;
            pal[pal_cnt++] = val;
        }

        /* length 5: k = 2 */
        for (int m = 1; m <= 9; m += 2) {
            int diff = 9 - m;
            if ((diff & 1) != 0) continue;
            int t = diff / 2;
            if (t < 2) continue;    /* min sum with two digits is 2 */
            for (int d1 = 1; d1 <= 9; ++d1)
                for (int d2 = 1; d2 <= 9; ++d2)
                    if (d1 + d2 == t) {
                        seq_t val = 0;
                        val = val * 10 + d1;
                        val = val * 10 + d2;
                        val = val * 10 + m;
                        val = val * 10 + d2;
                        val = val * 10 + d1;
                        pal[pal_cnt++] = val;
                    }
        }

        /* length 7: k = 3 */
        for (int m = 1; m <= 9; m += 2) {
            int diff = 9 - m;
            if ((diff & 1) != 0) continue;
            int t = diff / 2;
            if (t < 3) continue;    /* min sum with three digits is 3 */
            for (int d1 = 1; d1 <= 9; ++d1)
                for (int d2 = 1; d2 <= 9; ++d2)
                    for (int d3 = 1; d3 <= 9; ++d3)
                        if (d1 + d2 + d3 == t) {
                            seq_t val = 0;
                            val = val * 10 + d1;
                            val = val * 10 + d2;
                            val = val * 10 + d3;
                            val = val * 10 + m;
                            val = val * 10 + d3;
                            val = val * 10 + d2;
                            val = val * 10 + d1;
                            pal[pal_cnt++] = val;
                        }
        }

        /* length 9: k = 4 */
        for (int m = 1; m <= 9; m += 2) {
            int diff = 9 - m;
            if ((diff & 1) != 0) continue;
            int t = diff / 2;
            if (t < 4) continue;    /* min sum with four digits is 4 */
            for (int d1 = 1; d1 <= 9; ++d1)
                for (int d2 = 1; d2 <= 9; ++d2)
                    for (int d3 = 1; d3 <= 9; ++d3)
                        for (int d4 = 1; d4 <= 9; ++d4)
                            if (d1 + d2 + d3 + d4 == t) {
                                seq_t val = 0;
                                val = val * 10 + d1;
                                val = val * 10 + d2;
                                val = val * 10 + d3;
                                val = val * 10 + d4;
                                val = val * 10 + m;
                                val = val * 10 + d4;
                                val = val * 10 + d3;
                                val = val * 10 + d2;
                                val = val * 10 + d1;
                                pal[pal_cnt++] = val;
                            }
        }

        /* sort ascending */
        for (size_t i = 0; i < pal_cnt; ++i)
            for (size_t j = i + 1; j < pal_cnt; ++j)
                if (pal[i] > pal[j]) {
                    seq_t tmp = pal[i];
                    pal[i] = pal[j];
                    pal[j] = tmp;
                }
        initialized = true;
    }

    if (n == 0 || n > pal_cnt)
        return 0;          /* undefined for indices outside the sequence */
    return pal[n - 1];
}
