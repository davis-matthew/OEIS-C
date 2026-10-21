/*
 * A055419  Number of points in N^n of norm <= 4.
 * https://oeis.org/A055419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of solutions (x1,…,xn)∈N^n to x1²+…+xn² ≤ 16
 *            computed by polynomial exponentiation of (1 + x + x⁴ + x⁹ +
 *            x¹⁶), i.e. a(n) = Σ_{k=0}^{16} coeff_k( (1 + x + x⁴ + x⁹ +
 *            x¹⁶)^n )
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 29, u64 n <= 114,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static void poly_mul(const seq_t *a, const seq_t *b, seq_t *res)
{
    /* res initialized to zero before call */
    for (idx_t i = 0; i < 17; ++i) {
        if (a[i] == 0) continue;
        for (idx_t j = 0; j < 17 - i; ++j) { /* keep degree ≤16 */
            if (b[j] == 0) continue;
            res[i + j] += a[i] * b[j];
        }
    }
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    seq_t base[17] = { (seq_t)1, (seq_t)1, (seq_t)0, (seq_t)0,
                       (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)0,
                       (seq_t)0, (seq_t)1, (seq_t)0, (seq_t)0,
                       (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0,
                       (seq_t)1 };

    seq_t result[17], temp[17];
    for (idx_t i = 0; i < 17; ++i) {
        result[i] = 0;
        temp[i] = 0;
    }
    result[0] = 1;               /* identity polynomial */

    idx_t power = n;
    while (power > 0) {
        if (power & 1) {
            /* result = result * base */
            for (idx_t i = 0; i < 17; ++i) temp[i] = 0;
            poly_mul(result, base, temp);
            for (idx_t i = 0; i < 17; ++i) result[i] = temp[i];
        }
        /* base = base * base */
        for (idx_t i = 0; i < 17; ++i) temp[i] = 0;
        poly_mul(base, base, temp);
        for (idx_t i = 0; i < 17; ++i) base[i] = temp[i];

        power >>= 1;
    }

    seq_t total = 0;
    for (idx_t i = 0; i < 17; ++i) total += result[i];
    return total;
}
