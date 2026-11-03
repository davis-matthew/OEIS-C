/*
 * A170162  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^39 = I.
 * https://oeis.org/A170162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is given by the generating function G(t) = (t^39 + 2*t^38 +
 *            2*t^37 + … + 2*t + 1) / (1 - 7*t - 7*t^2 - … - 7*t^38 +
 *            28*t^39). The coefficients satisfy: • a(0) = 1 • for 1 ≤ n ≤ 38
 *            : a(n) = 2 + 7 * Σ_{k=0}^{n-1} a(k) • a(39) = 1 + 7 *
 *            Σ_{k=1}^{38} a(k) – 28 * a(0) • for n ≥ 40 : a(n) = 7*a(n‑...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coefficients of G(t) = (1 + 2t + ... + 2t^38 + t^39) / (1 - 7t - ... - 7t^38 + 28t^39),
   computed in order, the last 39 terms kept in a ring buffer (no allocation). */
seq_t a(idx_t n)
{
    seq_t r[40];                          /* r[i % 40] = a(i) */
    for (idx_t i = 0; i <= n; ++i) {
        seq_t v = (seq_t)(i == 0 ? 1 : i < 39 ? 2 : i == 39 ? 1 : 0);
        for (idx_t k = 1; k <= 38 && k <= i; ++k)
            v = v + (seq_t)7 * r[(i - k) % 40];
        if (i >= 39) v = v - (seq_t)28 * r[(i - 39) % 40];
        r[i % 40] = v;
    }
    return r[n % 40];
}
