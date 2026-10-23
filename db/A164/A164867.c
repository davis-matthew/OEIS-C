/*
 * A164867  Number of reduced words of length n in Coxeter group on 16
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A164867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=16, a(2)=240, a(3)=3600, a(4)=54000,
   a(5)=810000, a(6)=12150000, a(7)=182250000;
   a(8)=14*a(7)+14*a(6)+14*a(5)+14*a(4)+14*a(3)+14*a(2)+14*a(1)-105*a(0)+1;
   For n≥9: a(n)=14*a(n‑1)+14*a(n‑2)+14*a(n‑3)+14*a(n‑4)+14*a(n‑5)+14*a(n‑6)+14*a(n‑7)-105*a(n‑8); */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *dp = NULL;
    static size_t cap = 0;  /* allocated capacity */
    static idx_t last = 0;  /* index of largest computed term */

    /* allocate / enlarge cache if necessary */
    if (dp == NULL || cap <= (size_t)n) {
        size_t new_cap = cap ? cap * 2 : 8;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(dp, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();   /* allocation failed */
        dp = tmp;
        cap = new_cap;
        /* initialise the very first term if this is the first allocation */
        if (last == 0 && dp[0] == 0) dp[0] = 1;
    }

    /* if the requested term is already computed, just return it */
    if (n <= last) return dp[n];

    /* compute missing terms from last+1 up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        switch (i) {
            case 0: dp[0] = 1; break;
            case 1: dp[1] = 16; break;
            case 2: dp[2] = 240; break;
            case 3: dp[3] = 3600; break;
            case 4: dp[4] = 54000; break;
            case 5: dp[5] = 810000; break;
            case 6: dp[6] = 12150000; break;
            case 7: dp[7] = 182250000; break;
            case 8: {
                seq_t sum = 0;
                for (int k = 1; k <= 7; ++k)
                    sum += (seq_t)14 * dp[i - k];
                sum -= (seq_t)105 * dp[0];
                sum += 1;  /* contribution of the numerator term */
                dp[i] = sum;
                break;
            }
            default: {
                seq_t sum = 0;
                for (int k = 1; k <= 7; ++k)
                    sum += (seq_t)14 * dp[i - k];
                sum -= (seq_t)105 * dp[i - 8];
                dp[i] = sum;
            }
        }
    }

    last = n;
    return dp[n];
}
