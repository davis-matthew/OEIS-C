/*
 * A140661  Number of pairs (b,c) with the same prime factors, 1<=b<=c<=n.
 * https://oeis.org/A140661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{rad r} (k_r * (k_r + 1)) / 2 where k_r is the number
 *            of integers 1 ≤ x ≤ n with the same set of prime factors (i.e.
 *            having radical r)
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle indices < 1 */
    if (n < 1) return 0;

    /* ---------- 1. Sieve for smallest prime factors (spf) ---------- */
    size_t sz = (size_t)(n + 1);
    idx_t *spf = (idx_t *)calloc(sz, sizeof(idx_t));
    if (!spf) return 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            if (i * i <= n) {
                for (idx_t j = i * i; j <= n; j += i)
                    if (!spf[j]) spf[j] = i;
            }
        }
    }

    /* ---------- 2. Compute radical of each number ---------- */
    seq_t *rad = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!rad) { free(spf); return 0; }
    rad[1] = 1;
    for (idx_t i = 2; i <= n; ++i) {
        idx_t p = spf[i];
        idx_t m = i;
        while (m % p == 0) m /= p;
        rad[i] = (seq_t)rad[m] * (seq_t)p;
    }

    /* ---------- 3. Count numbers per radical ---------- */
    idx_t *freq = (idx_t *)calloc(sz, sizeof(idx_t));
    if (!freq) { free(spf); free(rad); return 0; }
    for (idx_t i = 1; i <= n; ++i)
        freq[(size_t)rad[i]]++;

    /* ---------- 4. Compute the answer ---------- */
    seq_t sum = 0;
    for (idx_t r = 1; r <= n; ++r) {
        idx_t k = freq[r];
        if (k)
            sum += (seq_t)k * ((seq_t)k + 1) / 2;
    }

    /* ---------- 5. Free memory ---------- */
    free(spf);
    free(rad);
    free(freq);

    return sum;
}
