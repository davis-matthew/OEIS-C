/*
 * A095143  Triangle, read by rows, formed by reading Pascal's triangle
 *          (A007318) mod 9.
 * https://oeis.org/A095143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binomial(row_of_index(n), n - row_prefix(n)) mod 9 where
 *            the triangle is read row by row from Pascal's triangle
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 105 terms through n = 104 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute C(n,k) modulo 9, using only operations allowed on seq_t. */
static seq_t
binomial_mod9(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;

    seq_t num_mod   = 1;
    seq_t denom_mod = 1;
    int   exp3      = 0;          /* exponent of 3 in the binomial coeff */

    for (idx_t t = 1; t <= k; ++t) {
        idx_t num = n - k + t;
        idx_t den = t;

        /* Strip factors of 3 from numerator and denominator. */
        while (num % 3 == 0) {
            num /= 3;
            exp3++;
        }
        while (den % 3 == 0) {
            den /= 3;
            exp3--;
        }

        /* Multiply modulo 9. Values stay < 81. */
        num_mod   = (num_mod   * (seq_t)(num   % 9)) % (seq_t)9;
        denom_mod = (denom_mod * (seq_t)(den   % 9)) % (seq_t)9;
    }

    /* If the power of 3 is at least 2, the coefficient is multiple of 9. */
    if (exp3 >= 2) return 0;

    /* Modulo 9, denom_mod is now coprime to 9. Find its inverse. */
    static const int inverse_mod9[9] = {0, 1, 5, 0, 7, 2, 0, 4, 8};
    uint64_t d = (uint64_t)(denom_mod % (seq_t)9);   /* 0 <= d < 9 */
    int inv_d  = inverse_mod9[d];
    if (inv_d == 0) return 0;                       /* should never happen */

    seq_t rem = (num_mod * (seq_t)inv_d) % (seq_t)9;

    if (exp3 == 1)
        rem = (rem * (seq_t)3) % (seq_t)9;

    return rem;
}

/* Find the row index i such that  i*(i+1)/2 <= n < (i+1)*(i+2)/2. */
static inline idx_t
row_of_index(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;
    /* First grow an upper bound. */
    while (true) {
        unsigned __int128 t = (unsigned __int128)high * (high + 1) / 2;
        if (t > n) break;
        high <<= 1;
    }
    low = high >> 1;
    /* Binary search between low and high. */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

/* OEIS sequence A095143. */
seq_t
a(idx_t n)
{
    idx_t row = row_of_index(n);
    /* prefix = sum_{i=0}^{row-1} (i+1) = row*(row+1)/2  */
    idx_t prefix = (idx_t)((unsigned __int128)row * (row + 1) / 2);
    idx_t col = n - prefix;                /* column index within the row */
    return binomial_mod9(row, col);
}
