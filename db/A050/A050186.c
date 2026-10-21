/*
 * A050186  Triangular array T read by rows: T(h,k) = number of binary words
 *          of k 1's and h-k 0's which are not a juxtaposition of 2 or more
 *          identical subwords.
 * https://oeis.org/A050186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, nice
 * Formula:   a(n) = T(h,k), where (h,k) is the row‑and‑column of n in the
 *            triangular array, and T(h,k) = Σ_{d | gcd(h,k)} μ(d) * C(h/d ,
 *            k/d), with μ the Möbius function and C the binomial coefficient.
 *            The array is read row by row starting with the empty word (n =
 *            0)
 * Range:     u8 n <= 69, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   gcd_idx, row_from_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_idx.h"
#include "../utils/row_from_n.h"

/*--------------------------------------------------------------------
   Helper: Euclidean greatest common divisor for uint64_t */

/*--------------------------------------------------------------------
   Helper: Möbius μ(n).  Returns -1, 0, or 1. */
static inline int mu_of(idx_t n) {
    if (n == 1) return 1;
    int cnt = 0;
    if (n % 2 == 0) {
        cnt++;
        n /= 2;
        if (n % 2 == 0) return 0;     /* squared prime factor */
        while (n % 2 == 0) n /= 2;
    }
    for (idx_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            cnt++;
            n /= p;
            if (n % p == 0) return 0;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) cnt++;                  /* remaining prime factor */
    return (cnt & 1) ? -1 : 1;         /* even → +1, odd  → -1 */
}

/*--------------------------------------------------------------------
   Helper: multiplicative binomial coefficient C(n,k)  using seq_t. */
static inline seq_t binom_idx(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;   /* exact division */
    }
    return res;
}

/*--------------------------------------------------------------------
   Main function: returns the n‑th term of A050186. */
seq_t a(idx_t n) {
    idx_t h   = row_from_n(n);                       /* row number   */
    idx_t start = (idx_t)((unsigned __int128)h * (h + 1) / 2);  /* first index of row */
    idx_t k   = n - start;                           /* column number */

    if (h == 0 && k == 0) return (seq_t)1;          /* empty word */

    idx_t g = gcd_idx(h, k);
    seq_t sum_pos = 0, sum_neg = 0;

    for (idx_t d = 1; d * d <= g; ++d) {
        if (g % d == 0) {
            int mu = mu_of(d);
            if (mu != 0) {
                seq_t term = binom_idx(h / d, k / d);
                if (mu > 0) sum_pos += term; else sum_neg += term;
            }
            idx_t d2 = g / d;
            if (d2 != d) {
                int mu2 = mu_of(d2);
                if (mu2 != 0) {
                    seq_t term = binom_idx(h / d2, k / d2);
                    if (mu2 > 0) sum_pos += term; else sum_neg += term;
                }
            }
        }
    }
    return sum_pos - sum_neg;
}
