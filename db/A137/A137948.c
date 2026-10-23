/*
 * A137948  Triangle read by rows, A000012 * A136579.
 * https://oeis.org/A137948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (r+1-k) * k! where r = floor((sqrt(8n+1)-1)/2) and k = n
 *            - r*(r+1)/2. This yields the OEIS sequence A137948
 * Range:     u8 n <= 26, u16 n <= 52, u32 n <= 103, u64 n <= 251,
 *            u128 n <= 663, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while (1) {
        /* use 128‑bit intermediate to avoid overflow of the product */
        __uint128_t bound = (__uint128_t)(r + 1) * (r + 2) / 2;
        if (bound > n) break;
        ++r;
    }

    idx_t start = (idx_t)((__uint128_t)r * (r + 1) / 2);
    idx_t k = n - start;

    /* compute (r+1-k) * k! */
    seq_t value = (seq_t)(r + 1 - k);
    seq_t fact = 1;
    for (idx_t i = 1; i <= k; ++i) {
        fact *= (seq_t)i;
    }
    return value * fact;
}
