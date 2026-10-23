/*
 * A163171  Number of reduced words of length n in Coxeter group on 23
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A163171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*a(n‑1)+21*a(n‑2)+21*a(n‑3)-231*a(n‑4)+b_n where b_0=1,
 *            b_1=b_2=b_3=2, b_4=1, and b_n=0 for n≥5. Initial values: a(0)=1,
 *            a(1)=23, a(2)=506, a(3)=11132, a(4)=244651
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *arr = NULL;
    static idx_t len = 0;   /* number of computed terms in arr */

    /* initialise the table for n=0 */
    if (len == 0) {
        len = 1;
        arr = (seq_t *)malloc(len * sizeof(seq_t));
        if (!arr) return 0;     /* memory failure: return 0 */
        arr[0] = 1;
    }

    /* extend the table up to n if needed */
    if (n >= len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(arr, new_len * sizeof(seq_t));
        if (!tmp) { free(arr); arr = NULL; len = 0; return 0; }
        arr = tmp;

        for (idx_t i = len; i <= n; ++i) {
            seq_t s = 0;
            if (i >= 1) s += 21 * arr[i - 1];
            if (i >= 2) s += 21 * arr[i - 2];
            if (i >= 3) s += 21 * arr[i - 3];
            if (i >= 4) s -= 231 * arr[i - 4];

            switch (i) {
                case 0: s += 1; break;
                case 1: s += 2; break;
                case 2: s += 2; break;
                case 3: s += 2; break;
                case 4: s += 1; break;
            }
            arr[i] = s;
        }
        len = new_len;
    }

    return arr[n];
}
