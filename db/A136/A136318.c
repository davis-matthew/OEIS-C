/*
 * A136318  Integers whose binary representation contains exactly two 1s, the
 *          1s not being adjacent.
 * https://oeis.org/A136318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 2^k + 2^m, where k is the smallest integer ≥2 with
 *            k(k-1)/2 ≥ n and m = n - (k-1)(k-2)/2 - 1
 * Range:     u8 n <= 21, u16 n <= 105, u32 n <= 465, u64 n <= 1953,
 *            u128 n <= 8001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return 2^exp using repeated multiplication.  Works for all seq_t sizes. */
static inline seq_t pow2(idx_t exp)
{
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        r = r * (seq_t)2;
    return r;
}

/* Test whether k*(k-1)/2 ≥ n, using 128‑bit intermediate to avoid overflow. */
static inline int triangular_ge(idx_t k, idx_t n)
{
    return ((unsigned __int128)k * (k - 1) / 2) >= n;
}

/* Find the minimal k such that k(k-1)/2 ≥ n.  n ≥ 1. */
static idx_t find_exponent(idx_t n)
{
    idx_t low = 2, high = 2;

    while (!triangular_ge(high, n))
        high <<= 1;

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (triangular_ge(mid, n))
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Return the nth term of A136318 (1‑based index). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined for n = 0 */

    idx_t k = find_exponent(n);                   /* highest exponent  */
    idx_t prev = (unsigned __int128)(k - 1) * (k - 2) / 2;  /* counts before this k */
    idx_t m = n - prev - 1;                        /* lowest exponent   */

    return pow2(k) + pow2(m);
}
