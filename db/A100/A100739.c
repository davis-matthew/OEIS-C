/*
 * A100739  Inverse modulo 2 binomial transform of 6^n.
 * https://oeis.org/A100739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=0..n} (-1)^A010060(n-k)*mod(binomial(n, k), 2)*6^k
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Pre‑compute 6^k for k = 0 … n */
    seq_t *pow6 = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!pow6) return 0;          /* fatal error – never expected to happen */

    pow6[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        pow6[i] = pow6[i - 1] * (seq_t)6;

    seq_t pos = (seq_t)0;
    seq_t neg = (seq_t)0;

    /* Enumerate all k with binomial(n, k) odd, i.e. k ⊆ n in binary */
    idx_t k = n;
    for (;;)
    {
        idx_t diff = n - k;                      /* n – k   */
        int  parity = __builtin_popcountll(diff) & 1; /* A010060(n-k) */

        if (parity == 0)
            pos += pow6[k];   /* even number of ones → + */
        else
            neg += pow6[k];   /* odd  → – */

        if (k == 0) break;
        k = (k - 1) & n;     /* next subset of n */
    }

    free(pow6);
    return pos - neg;         /* pos ≥ neg for all n ≥ 0 */
}
