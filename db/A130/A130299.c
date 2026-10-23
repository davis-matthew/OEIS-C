/*
 * A130299  A130296 * A051340.
 * https://oeis.org/A130299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let r,c be defined by n = T_{r-1} + c, where T_k = k(k+1)/2 is
 *            the k‑th triangular number and 1 ≤ c ≤ r. Then a(n) = 2r‑1 if c
 *            == 1, r otherwise
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the k‑th triangular number safely using 128‑bit arithmetic. */
static inline idx_t triangular(idx_t k)
{
    return (idx_t)(((__uint128_t)k * (k + 1)) / 2);
}

seq_t a(idx_t n)
{
    /* Find r such that T_{r-1} < n ≤ T_r.  Use binary search. */
    idx_t low = 1, high = 1;
    while (triangular(high) < n)
        high <<= 1;                 /* double until high is large enough */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (triangular(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Column index c (1‑based).  T_{r-1} is the number of elements
       preceding row r.  */
    idx_t tPrev = triangular(r - 1);
    idx_t c = n - tPrev;          /* 1 ≤ c ≤ r */

    /* Compute the element according to the rule above. */
    seq_t val;
    if (c == 1)
        val = (seq_t)(2 * (seq_t)r - 1);
    else
        val = (seq_t)r;

    return val;
}
