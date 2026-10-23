/*
 * A100247  Slanted Catalan convolution table, read by rows of 2*n+1 terms in
 *          row n, where T(n,k) = C(n+2*k-[k/2],k)*(n-[k/2])/(n+2*k-[k/2]).
 * https://oeis.org/A100247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is obtained by reading the array T(r,k) row‑by‑row * where
 *            T(r,k) = C(r+2*k-[k/2], k) * (r-[k/2]) / (r+2*k-[k/2]). * For a
 *            given index n, let r = ⌊√n⌋ and k = n - r². * Then a(n) =
 *            T(r,k). The offset is 0
 * Range:     u8 n <= 13, u16 n <= 33, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   gcd_seq, integer_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_seq.h"
#include "../utils/integer_sqrt.h"

static seq_t binom(idx_t N, idx_t K)
{
    if (K > N - K) K = N - K;
    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i) {
        seq_t num = (seq_t)(N - K + i);
        seq_t den = (seq_t)i;
        seq_t g = gcd_seq(num, den);
        num /= g;
        den /= g;
        g = gcd_seq(res, den);
        res /= g;
        den /= g;
        res = res * num;
        if (den != 1) res /= den; /* exact division */
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* T(0,0) = 1 */

    idx_t r = integer_sqrt(n);
    idx_t k = n - (idx_t)((unsigned __int128)r * r);

    idx_t s = k / 2;                          /* floor(k/2) */
    idx_t a_val = r + 2 * k - s;              /* n + 2*k - [k/2] */

    seq_t bin = binom(a_val, k);
    seq_t num = bin * ((seq_t)r - (seq_t)s);
    seq_t den = (seq_t)a_val;

    return num / den;
}
