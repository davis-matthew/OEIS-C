/*
 * A205853  The least number j such that 5 divides s(k)-s(j), where
 *          k(n)=A205852(n) and s(k) denotes the (k+1)-st Fibonacci number.
 * https://oeis.org/A205853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th j (in order of k, then increasing j) with j<k and 5
 *            | s(k)-s(j), s(m)=Fibonacci(m+1); computed by direct
 *            counting/search
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A205853_res(idx_t k)
{
    static int tab[20];
    static int init = 0;
    if (!init) {
        int f0 = 0, f1 = 1; /* F(0), F(1) */
        for (int i = 0; i < 20; i++) {
            /* tab[i] = F(i+1) mod 5 */
            tab[i] = f1;
            int t = (f0 + f1) % 5;
            f0 = f1;
            f1 = t;
        }
        init = 1;
    }
    return tab[(int)(k % 20)];
}

seq_t a(idx_t n)
{
    idx_t count[5] = {0, 0, 0, 0, 0};
    idx_t cumulative = 0;
    for (idx_t k = 1; ; ++k) {
        int r = A205853_res(k);
        idx_t occ = count[r];
        if (occ > 0) {
            if (cumulative + occ >= n) {
                idx_t m = n - cumulative; /* m-th j with residue r, 1-based */
                idx_t seen = 0;
                for (idx_t j = 1; j < k; ++j) {
                    if (A205853_res(j) == r) {
                        ++seen;
                        if (seen == m) return (seq_t)j;
                    }
                }
            }
            cumulative += occ;
        }
        count[r]++;
    }
}
