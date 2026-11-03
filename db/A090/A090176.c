/*
 * A090176  Expansion of (1+x^9)/((1-x^4)*(1-x^6)*(1-x^12)).
 * https://oeis.org/A090176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of solutions to 4*i + 6*j + 12*k = n plus the same
 *            for n-9, where i, j, k are non‑negative integers
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Validated: partial, pass, 85 terms through n = 84 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t count_c(idx_t m)
{
    seq_t total = 0;
    /* Only residues 0 and 2 modulo 4 are possible. */
    idx_t r = m & 3;          /* m % 4 */
    if (r == 1 || r == 3)     return 0;

    idx_t maxK = m / 12;
    for (idx_t k = 0; k <= maxK; ++k) {
        idx_t m1 = m - 12 * k;   /* remaining after choosing k */
        idx_t jmax = m1 / 6;     /* maximum j for this k */

        if (r == 0) {           /* need j even */
            total += (jmax / 2) + 1;
        } else {                 /* r == 2 -> need j odd */
            if (jmax >= 1) {
                total += ((jmax - 1) / 2) + 1;
            }
        }
    }
    return total;
}

seq_t a(idx_t n)
{
    seq_t ans = count_c(n);
    if (n >= 9) ans += count_c(n - 9);
    return ans;
}
