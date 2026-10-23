/*
 * A119937  Triangle of numbers related to the spectrum of the hydrogen (H)
 *          atom.
 * https://oeis.org/A119937
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = ((m^2 - n^2) * L) / (m^2 * n^2) where m is the row index
 *            chosen by m(m-1)/2 >= n-1 and (m-1)(m-2)/2 < n-1 (i.e. n = 1 +
 *            (m-1)(m-2)/2 + n, 1 ≤ n ≤ m-1) L = lcm_{k=1..m-1} ( m^2 * k^2 /
 *            gcd(m^2*k^2 , m^2 - k^2) ) (the denominators of the reduced
 *            fractions r(m,k)). This is equivalent to the O...
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   gcd, uint64_from_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/gcd.h"
#include "../utils/uint64_from_idx.h"

/* If seq_t is a type larger than 64 bits (e.g. unsigned __int128
   or an arbitrary‑precision class) the operators +, -, *, /, %,
   comparisons, <<, >>, and unary minus are available.  The only
   external routine used is gcd(seq_t a, seq_t b), which returns
   the greatest common divisor of its arguments. */

/* compute a(n)  (global index n ≥ 2) */
seq_t a (idx_t n)
{
    if (n < 2) return 0;                 /* out of bounds – undefined */

    /* ---------- locate the row m and the local column n (1‑based) ---------- */
    idx_t k  = n - 1;                    /* shift so the first term is k = 1 */
    long double sq = sqrt((long double)(1 + 8 * k));
    idx_t     root = (idx_t)sq;          /* floor of the square root */
    idx_t     m    = (1 + root) / 2;
    if (m * (m - 1) / 2 < k) ++m;        /* ensure m(m-1)/2 ≥ k */

    idx_t prev_sum = (m - 1) * (m - 2) / 2;
    idx_t n_local  = n - prev_sum - 1;    /* 1 <= n_local <= m-1 */

    /* ---------- compute the LCM of the reduced denominators for this row ---------- */
    seq_t L     = (seq_t)1;
    seq_t m2seq = (seq_t)m * (seq_t)m;   /* m² */

    for (idx_t kk = 1; kk < m; ++kk) {
        seq_t k2seq = (seq_t)kk * (seq_t)kk;          /* k² */
        seq_t base  = m2seq * k2seq;                  /* m²·k²   */
        seq_t num   = m2seq - k2seq;                  /* m²‑k²   */
        seq_t g     = gcd(base, num);                 /* gcd(m²k² , m²‑k²)   */
        seq_t denom = base / g;                       /* reduced denominator */
        seq_t g2    = gcd(L, denom);                  /* lcm step */
        L          = (L / g2) * denom;
    }

    /* ---------- compute the value of the requested entry ---------- */
    seq_t n2seq = (seq_t)n_local * (seq_t)n_local;
    seq_t base  = m2seq * n2seq;                     /* m²·n²   */
    seq_t num   = m2seq - n2seq;                     /* m²‑n²   */

    /* Reduce the fraction (num * L) / base : avoid overflow on 64‑bit builds */
    seq_t g1    = gcd(num, base);
    seq_t num_red   = num / g1;
    seq_t base_red  = base / g1;
    seq_t g2    = gcd(L, base_red);
    L          = L / g2;
    base_red   = base_red / g2;

    seq_t result = (num_red * L) / base_red;          /* final integer */

    return result;
}
