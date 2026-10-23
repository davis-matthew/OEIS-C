/*
 * A038274  Triangle whose (i,j)-th entry is binomial(i,j)*7^(i-j)*8^j.
 * https://oeis.org/A038274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 7^(i-j) * 8^j where * i and j are
 *            determined from the row‑major ordering of the * triangle with
 *            entries binomial(i,j)*7^(i-j)*8^j. * Specifically, for a given
 *            n: * i = floor((sqrt(8*n+1)-1)/2) * j = n - i*(i+1)/2 * This
 *            yields the desired value
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute integer square root of 8*n+1 via binary search */
    idx_t x = (idx_t)((unsigned __int128)8 * n + 1);
    idx_t lo = 0, hi = (x >> 1) | 1, mid, res = 0;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x) { res = mid; break; }
        if (sq < x) { lo = mid + 1; res = mid; }
        else        { hi = mid - 1; }
    }
    idx_t sqrt_val = res;

    /* Determine row i and column j */
    idx_t i = (sqrt_val - 1) / 2;
    idx_t start = i * (i + 1) / 2;
    idx_t j = n - start;

    /* Compute binomial(i,j) multiplicatively */
    idx_t k = j;
    if (k > i - j) k = i - j;
    seq_t binom = (seq_t)1;
    for (idx_t t = 1; t <= k; ++t) {
        binom = binom * (seq_t)(i - k + t);
        binom = binom / (seq_t)t;
    }

    /* Compute powers of 7 and 8 */
    idx_t exp7 = i - j;
    seq_t pow7 = (seq_t)1;
    for (idx_t t = 0; t < exp7; ++t) pow7 = pow7 * (seq_t)7;

    idx_t exp8 = j;
    seq_t pow8 = (seq_t)1;
    for (idx_t t = 0; t < exp8; ++t) pow8 = pow8 * (seq_t)8;

    /* Return the product */
    return binom * pow7 * pow8;
}
