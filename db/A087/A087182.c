/*
 * A087182  Number partition numbers <= P(n) of the form 3*k+2 (P = A000041).
 * https://oeis.org/A087182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { m | 0 ≤ m ≤ n : p(m) ≡ 2 (mod 3) }, where p(m) is the
 *            partition * number A000041. The partition numbers are computed
 *            modulo 3 using the * pentagonal‑number recurrence; a(n) is
 *            accumulated as we compute the * partition numbers. This requires
 *            only O(n·sqrt(n)) arithmetic with * 16‑bit in...
 * Range:     u8 n <= 712, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static storage for values computed so far */
    static uint8_t  *pid = NULL;   /* p(m) mod 3, 0..max_n */
    static seq_t    *cnt = NULL;   /* a(m), cumulative count */
    static idx_t    max_n = 0;     /* highest n already rendered */

    /* enlarge arrays if necessary */
    if (n >= max_n) {
        idx_t newsize = (n + 1);
        pid = (uint8_t *)realloc(pid, newsize * sizeof(uint8_t));
        cnt = (seq_t *)realloc(cnt, newsize * sizeof(seq_t));

        /* initialize partition number p(0)=1 */
        if (max_n == 0) {
            pid[0] = 1 % 3;   /* 1 */
            cnt[0] = 0;       /* p(0) ≠ 2, so a(0)=0 */
            max_n = 1;
        }

        /* compute partitions from current max to n */
        for (idx_t j = max_n; j <= n; ++j) {
            int sum = 0;        /* sign‑weighted sum in ordinary integers */

            for (idx_t k = 1;; ++k) {
                /* Generalised pentagonal numbers: k(3k‑1)/2 and k(3k+1)/2 */
                idx_t g1 = k * (3 * k - 1) / 2;
                if (g1 > j) break;
                int sign = (k & 1) ? 1 : -1;  /* (+, –, +, – …) */
                sum += sign * pid[j - g1];

                idx_t g2 = k * (3 * k + 1) / 2;
                if (g2 > j) continue;
                sum += sign * pid[j - g2];
            }

            /* reduce modulo 3, ensure positive */
            int mod = sum % 3;
            if (mod < 0) mod += 3;
            pid[j] = (uint8_t)mod;

            /* accumulate count of values ≡ 2 (mod 3) */
            seq_t prev = cnt[j - 1];
            cnt[j] = prev + (pid[j] == 2);
        }
        max_n = n + 1;
    }
    return cnt[n];
}
