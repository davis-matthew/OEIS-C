/*
 * A232924  Number of 5Xn 0..2 arrays with no element x(i,j) adjacent to value
 *          2-x(i,j) horizontally, diagonally or antidiagonally.
 * https://oeis.org/A232924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 31*a(n-2) - 97*a(n-3) - 163*a(n-4) +
 *            417*a(n-5) + 93*a(n-6) - 514*a(n-7) + 192*a(n-8) + 72*a(n-9) -
 *            36*a(n-10) (valid for n > 11) Initial terms (n = 1..11): a(1) =
 *            243 a(2) = 486 a(3) = 5118 a(4) = 37008 a(5) = 316800 a(6) =
 *            2515716 a(7) = 20706696 a(8) = 167584476 a(9) = 136743...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache holding the sequence up to the highest index requested */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;            /* last index stored (0 if empty) */
    static bool cache_allocated = false;

    /* -----------------------------------------------
       Compute base terms a(1)..a(11) via DP, once.
       ----------------------------------------------- */
    static seq_t base[12];                  /* base[0] unused */
    static bool base_ready = false;

    if (!base_ready) {
        /* Compatibility between column states */
        static unsigned char state_digit[243][5];
        static bool compat[243][243];
        static bool compat_ready = false;

        if (!compat_ready) {
            /* Pre‑compute decimal digits (base‑3) for each state */
            for (int st = 0; st < 243; ++st) {
                int v = st;
                for (int r = 0; r < 5; ++r) {
                    state_digit[st][r] = (unsigned char)(v % 3);
                    v /= 3;
                }
            }
            /* Build compatibility matrix */
            for (int t = 0; t < 243; ++t) {
                for (int s = 0; s < 243; ++s) {
                    bool incompatible = false;
                    for (int r = 0; r < 5 && !incompatible; ++r) {
                        unsigned char tv = state_digit[t][r];
                        for (int d = -1; d <= 1 && !incompatible; ++d) {
                            int r2 = r + d;
                            if (r2 < 0 || r2 >= 5) continue;
                            unsigned char sv = state_digit[s][r2];
                            if (tv + sv == 2) incompatible = true;
                        }
                    }
                    compat[t][s] = !incompatible;
                }
            }
            compat_ready = true;
        }

        /* DP to compute a(1)..a(11) */
        seq_t cur[243];
        seq_t nxt[243];
        for (int st = 0; st < 243; ++st) cur[st] = (seq_t)1;   /* a(1) */
        /* Sum over all states for a(1) */
        seq_t sum = 0;
        for (int st = 0; st < 243; ++st) sum += cur[st];
        base[1] = sum;

        for (int col = 2; col <= 11; ++col) {
            for (int st = 0; st < 243; ++st) nxt[st] = (seq_t)0;
            for (int t = 0; t < 243; ++t) {
                seq_t vt = cur[t];
                if (vt == 0) continue;
                for (int s = 0; s < 243; ++s) {
                    if (compat[t][s]) nxt[s] += vt;
                }
            }
            for (int st = 0; st < 243; ++st) cur[st] = nxt[st];
            seq_t ssum = 0;
            for (int st = 0; st < 243; ++st) ssum += cur[st];
            base[col] = ssum;
        }
        base_ready = true;
    }

    /* -----------------------------------------------
       Allocate or grow the cache if necessary.
       ----------------------------------------------- */
    if (n > cache_size) {
        if (!cache) {
            cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
            cache_allocated = true;
        } else {
            cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        }

        /* Copy known base terms into cache */
        idx_t upto = (n < 11 ? n : 11);
        for (idx_t i = 1; i <= upto; ++i) cache[i] = base[i];
        cache_size = upto;

        /* Compute further terms (n > 11) via recurrence */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t a1  = cache[i - 1];
            seq_t a2  = cache[i - 2];
            seq_t a3  = cache[i - 3];
            seq_t a4  = cache[i - 4];
            seq_t a5  = cache[i - 5];
            seq_t a6  = cache[i - 6];
            seq_t a7  = cache[i - 7];
            seq_t a8  = cache[i - 8];
            seq_t a9  = cache[i - 9];
            seq_t a10 = cache[i - 10];
            seq_t next =
                6 * a1 + 31 * a2 - 97 * a3 - 163 * a4 + 417 * a5
              + 93 * a6 - 514 * a7 + 192 * a8 + 72 * a9 - 36 * a10;
            cache[i] = next;
        }
        cache_size = n;
    }

    return cache[n];
}
