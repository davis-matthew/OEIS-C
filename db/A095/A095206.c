/*
 * A095206  Let n = d(1),d(2),...,d(m) be the decimal representation of n and
 *          f(k) be the number obtained by deleting all occurrences of digit k
 *          from n, then a(n) = Sum_{k=1..m, d(k)!=0} f(d(k)) mod d(k).
 * https://oeis.org/A095206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Σ_{pos i with digit d_i ≠ 0} (value obtained by removing
 *               all occurrences of d_i from decimal representation of n) mod d_i
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* store decimal digits of n in forward order (most significant first) */
    int digits[20];   /* enough for 64‑bit unsigned numbers */
    int m = 0;
    idx_t tmp = n;
    if (tmp == 0) {
        digits[0] = 0;
        m = 1;
    } else {
        while (tmp) {
            digits[m++] = tmp % 10;
            tmp /= 10;
        }
        /* reverse to get most significant digit first */
        for (int i = 0; i < m / 2; ++i) {
            int t = digits[i];
            digits[i] = digits[m - 1 - i];
            digits[m - 1 - i] = t;
        }
    }

    seq_t sum = 0;

    /* iterate over each position j */
    for (int j = 0; j < m; ++j) {
        int d = digits[j];
        if (d == 0) continue;              /* skip zero digits */

        /* compute value after deleting all digits equal to d, modulo d */
        int mod = 0;
        for (int k = 0; k < m; ++k) {
            if (digits[k] == d) continue; /* skip this digit */
            mod = (mod * 10 + digits[k]) % d;
        }
        sum += (seq_t)mod;
    }

    return sum;
}
