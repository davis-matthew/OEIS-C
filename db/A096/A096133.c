/*
 * A096133  Triangle T(j,k) = (j^k) mod (j*k) for 1 <= k <= j, read by rows.
 * https://oeis.org/A096133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = (j^k) mod (j*k) where j is the row and k is the position
 *            in that row in the triangle T(j,k) = j^k mod (j*k). The row
 *            index j and column k are determined from n by the triangular
 *            number formula: n ≤ j*(j+1)/2 and k = n - (j-1)*j/2
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t modpow(seq_t base, idx_t exp, seq_t mod)
{
    seq_t result = 1 % mod;
    seq_t b = base % mod;
    while (exp > 0) {
        if (exp & 1) result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* Off‑by‑one robustness */

    /* Find row j and column k such that n = (j-1)*j/2 + k, 1 ≤ k ≤ j */
    idx_t j = 1;
    idx_t cumulative = 0;
    while (cumulative + j < n) {
        cumulative += j;
        ++j;
    }
    idx_t k = n - cumulative;

    seq_t base = (seq_t)j;
    seq_t mod  = (seq_t)j * (seq_t)k;   /* j*k fits in seq_t */

    return modpow(base, k, mod);
}
