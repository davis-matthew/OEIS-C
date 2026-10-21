/*
 * A064344  Generalized Catalan numbers C(6,6; n).
 * https://oeis.org/A064344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A064344: Generalized Catalan numbers C(6,6;n)
   Recurrence (n ≥ 3):
       5*(n‑1) * a(n) = 6*(119*n-300) * a(n‑1) + 432*(2*n-3) * a(n‑2)
   Initial values:
       a(0) = 1,  a(1) = 1,  a(2) = 12
   All operations are performed using seq_t, which is an unsigned
   integer type large enough to hold the result.  Intermediate
   computations use unsigned __int128 to avoid overflow in the
   64‑bit build.
*/

seq_t a(idx_t n) {
    /* Cache the computed terms up to 10000 to keep each call O(1) after
       the first in that range.  The cache is initialized on the first
       call. */
    static seq_t cache[10001];
    static idx_t cached_up_to = 2;   /* index of largest cached value */

    /* Initialise cache[0..2] the first time the function is called. */
    if (cached_up_to == 2 && cache[0] != (seq_t)1) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)12;
    }

    /* If the requested index is already cached, return it immediately. */
    if (n <= cached_up_to)
        return cache[n];

    /* Compute terms iteratively until the desired index is reached. */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        if (i == 0 || i == 1) {
            cache[i] = (seq_t)1;
        } else if (i == 2) {
            cache[i] = (seq_t)12;
        } else {
            /* Use 128‑bit intermediate to keep the product exact. */
            unsigned __int128 coeff1 = 6ULL * (((unsigned __int128)119 * i) - 300);
            unsigned __int128 term1   = coeff1 * (unsigned __int128)cache[i-1];

            unsigned __int128 coeff2 = 432ULL * (((unsigned __int128)2 * i) - 3);
            unsigned __int128 term2   = coeff2 * (unsigned __int128)cache[i-2];

            unsigned __int128 numerator   = term1 + term2;
            unsigned __int128 denominator = 5ULL * (((unsigned __int128)i) - 1);

            cache[i] = (seq_t)(numerator / denominator);
        }
    }
    cached_up_to = n;
    return cache[n];
}
