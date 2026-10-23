/*
 * A165726  Number of reduced words of length n in Coxeter group on 50
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   From the generating function (t^9 + 2t^8 + … + 2t + 1) / (1176
 *            t^9 – 48t^8 – … – 48t + 1). The coefficients satisfy the linear
 *            recurrence a(n) = 48*(a(n-1)+…+a(n-8)) – 1176*a(n-9) + c(n),
 *            where c(0)=c(9)=1, c(1…8)=2, and c(n)=0 for n>9
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t last   = 0;

    if (!cache) {
        cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache) exit(1);
        cache[0] = (seq_t)1;
        last = 0;
    }

    if ((size_t)n >= cap) {
        size_t newcap = cap ? cap : 1;
        while ((size_t)n >= newcap) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
        if (!tmp) exit(1);
        cache = tmp;
        cap = newcap;
    }

    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t sum = 0;

        if (i >= 1) sum += (seq_t)48 * cache[i - 1];
        if (i >= 2) sum += (seq_t)48 * cache[i - 2];
        if (i >= 3) sum += (seq_t)48 * cache[i - 3];
        if (i >= 4) sum += (seq_t)48 * cache[i - 4];
        if (i >= 5) sum += (seq_t)48 * cache[i - 5];
        if (i >= 6) sum += (seq_t)48 * cache[i - 6];
        if (i >= 7) sum += (seq_t)48 * cache[i - 7];
        if (i >= 8) sum += (seq_t)48 * cache[i - 8];
        if (i >= 9) sum -= (seq_t)1176 * cache[i - 9];

        seq_t add = 0;
        if (i == 0 || i == 9) add = 1;
        else if (i <= 8) add = 2;
        sum += add;

        cache[i] = sum;
    }

    last = n;
    return cache[n];
}
