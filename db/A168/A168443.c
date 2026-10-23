/*
 * A168443  Triangle, T(n,k) = number of compositions a(1),...,a(k) of n, such
 *          that a(i+1) <= a(i) + 1 for 1 <= i < k.
 * https://oeis.org/A168443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth element of the triangle of restricted
 *            compositions (OEIS A168443), read row‑wise. The triangle entry
 *            T(n,k) counts compositions of n into k parts a(1)…a(k) with
 *            a(i+1) ≤ a(i)+1
 * Range:     u8 n <= 97, u16 n <= 289, u32 n <= 1014, u64 n <= 3703,
 *            u128 n <= 8702, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Intuition: precompute the triangle by dynamic programming up to the
   largest row needed for n ≤ 10000 (row 141). */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* the maximum row that covers indices up to 2·10⁴:
       150·151/2 = 11 325 > 10 000, enough for the required limit. */
    enum { MAX_R = 150 };
    static bool initialized = false;
    static seq_t f[MAX_R+1][MAX_R+1][MAX_R+1];
    static seq_t T[MAX_R+1][MAX_R+1];

    if (!initialized) {
        /* base: one part (k=1) */
        for (int last = 1; last <= MAX_R; ++last) {
            f[last][1][last] = 1;
            T[last][1] = 1;
        }

        /* DP for k ≥ 2 */
        for (int k = 2; k <= MAX_R; ++k) {
            for (int n = k; n <= MAX_R; ++n) {
                seq_t total = 0;
                int maxLast = n - (k - 1);
                for (int last = 1; last <= maxLast; ++last) {
                    seq_t sum = 0;
                    int maxPrev = n - last - (k - 2);
                    for (int prev = last - 1; prev <= maxPrev; ++prev) {
                        sum += f[n - last][k - 1][prev];
                    }
                    f[n][k][last] = sum;
                    total += sum;
                }
                T[n][k] = total;
            }
        }
        initialized = true;
    }

    /* Locate (row,column) that corresponds to index n in row‑wise order. */
    idx_t r = 1;
    idx_t cum = 0;
    while (cum + r < n) {
        cum += r;
        ++r;
    }
    idx_t c = n - cum;   /* 1 ≤ c ≤ r */

    if (r > MAX_R) return 0;   /* index outside pre‑computed range */
    return T[(int)r][(int)c];
}
