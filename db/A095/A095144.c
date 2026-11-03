/*
 * A095144  Triangle, read by rows, formed by reading Pascal's triangle
 *          (A007318) mod 11.
 * https://oeis.org/A095144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = C(i, j) mod 11 where i = floor((sqrt(8n+1)-1)/2), j = n -
 *            i(i+1)/2
 * Range:     u8 n <= 10010, u16 n <= 10010, u32 n <= 10010, u64 n <= 10010,
 *            u128 n <= 10010, big any n
 * Validated: standard, pass, 121 terms through n = 9682 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns binomial(n, k) modulo 11 using Lucas's theorem.
   The arguments are idx_t, but the return type is seq_t. */
static seq_t binom_mod11(idx_t n, idx_t k)
{
    if (k > n)
        return 0;

    seq_t result = 1;
    while (n > 0 || k > 0) {
        idx_t ni = n % 11;
        idx_t ki = k % 11;
        if (ki > ni)
            return 0;

        /* Compute C(ni, ki) exactly (ni, ki <= 10). */
        seq_t comb = 1;
        for (idx_t i = 1; i <= ki; ++i)
            comb = comb * (ni - i + 1) / i;

        /* Combine modulo 11. */
        result = (result * (comb % 11)) % 11;

        n /= 11;
        k /= 11;
    }
    return result;
}
/* Computes the triangular row index i for a given linear index n
   by solving the quadratic equation i^2 + i - 2n <= 0. */
static idx_t triangular_row(idx_t n)
{
    /* floor(sqrt(1 + 8n)) fits in 32 bits for any n <= UINT64_MAX */
    unsigned __int128 val = (unsigned __int128)8 * n + 1;
    idx_t low = 0;
    idx_t high = 0x100000000ULL;          /* 2^32 */

    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= val)
            low = mid;
        else
            high = mid;
    }

    /* i = (sqrt(1 + 8n) - 1) / 2, floor division */
    return (low - 1) >> 1;
}

/* Public sequence function required by the build. */
seq_t a(idx_t n)
{
    idx_t i = triangular_row(n);
    idx_t row_start = (unsigned __int128)i * (i + 1) / 2;
    idx_t j = n - row_start;
    return binom_mod11(i, j);
}
