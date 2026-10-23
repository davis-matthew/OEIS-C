/*
 * A025750  5th-order Patalan numbers (generalization of Catalan numbers).
 * https://oeis.org/A025750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 1; a(n) = 5^(n‑1) * ∏_{j=2..n} (5*j-6) / n! for
 *            n ≥ 2 This recurrence can be written as a(n) = a(n‑1) * 5 *
 *            (5*n‑6) / n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* helper: Euclidean gcd for idx_t arguments (works for C11 and C++17) */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;          /* a(0) = a(1) = 1 */

    seq_t res = (seq_t)1;                  /* equal to a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        /* factor = 5 * (5*i - 6) */
        seq_t factor = (seq_t)5 * ((seq_t)5 * (seq_t)i - (seq_t)6);

        /* gcd(i,30) = gcd(factor, i)  (factor ≡ –30 (mod i)) */
        idx_t g = gcd_idx(i, 30);

        factor /= (seq_t)g;                /* exact division */
        idx_t i_div = i / g;

        /* For seq_t types larger than 64‑bit the multiplication is safe
           directly; for 64‑bit we rely on the reductions above to keep
           the intermediate product within the range. */
        res = (res * factor) / i_div;
    }
    return res;
}
