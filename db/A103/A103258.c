/*
 * A103258  G.f. = theta_4(0,x^4)/theta_4(0,x).
 * https://oeis.org/A103258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Σ_{k=1..n} (Σ_{d|k} d · b(d)) · a(n−k) where b(d)
 *            = [2,1,2,−1,2,1,2,0] for d mod 8 = 1..8 respectively
 * Range:     u8 n <= 11, u16 n <= 32, u32 n <= 101, u64 n <= 345,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* using unsigned 64‑bit arithmetic for the result;  
   intermediate sums are performed in unsigned 128‑bit to avoid overflow. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    /* Pre‑compute the coefficients s[k] = Σ_{d|k} d·b(d)  for k=1..n. */
    int64_t *s = (int64_t*)malloc((n + 1) * sizeof(int64_t));
    if (!s) return (seq_t)0;   /* out‑of‑memory, return 0 as a safe default */
    for (idx_t i = 0; i <= n; ++i) s[i] = 0;

    for (idx_t d = 1; d <= n; ++d) {
        int b;
        switch (d & 7) { /* d % 8 */
            case 1: b =  2; break;
            case 2: b =  1; break;
            case 3: b =  2; break;
            case 4: b = -1; break;
            case 5: b =  2; break;
            case 6: b =  1; break;
            case 7: b =  2; break;
            default: continue; /* b = 0 when d % 8 == 0 */
        }
        int64_t add = (int64_t)d * (int64_t)b;
        for (idx_t j = d; j <= n; j += d)
            s[j] += add;
    }

    /* Array to store a[0..n] */
    seq_t *a_arr = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!a_arr) { free(s); return (seq_t)0; }
    a_arr[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        /* compute a[i] = (Σ_{k=1..i} s[k]·a[i−k]) / i */
        __int128 sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            if (s[k] == 0) continue;
            sum += (__int128)s[k] * (__int128)a_arr[i - k];
        }
        a_arr[i] = (seq_t)(sum / (__int128)i);
    }

    seq_t result = a_arr[n];
    free(a_arr);
    free(s);
    return result;
}
