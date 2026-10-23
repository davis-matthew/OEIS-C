/*
 * A131923  Triangle read by rows: T(n,k) = binomial(n,k) + n.
 * https://oeis.org/A131923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(r,k) + r * The index n is mapped to the triangle
 *            entry T(r,k) where r is the row * such that r(r+1)/2 ≤ n <
 *            (r+1)(r+2)/2 and k = n - r(r+1)/2. The value * of the entry is
 *            the binomial coefficient C(r,k) plus the row number r
 * Range:     u8 n <= 59, u16 n <= 197, u32 n <= 646, u64 n <= 2376,
 *            u128 n <= 5049, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Determine binomial coefficient multiplicatively, avoiding overflow.
 * For 64‑bit seq_t we use a 128‑bit intermediate; otherwise we rely on
 * the seq_t type’s arithmetic (e.g. an arbitrary‑precision integer). */
#if defined(__SIZEOF_INT128__)
#  define HAVE_INT128 1
#else
#  define HAVE_INT128 0
#endif

static seq_t binomial_multiplicative(idx_t n, idx_t k) {
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        idx_t mult = n - k + i;
#if HAVE_INT128
        __int128 prod = (__int128)res * mult;
        res = (seq_t)(prod / i);
#else
        seq_t prod = res * mult;
        res = prod / i;
#endif
    }
    return res;
}

seq_t a(idx_t n) {
    /* Find the largest r with r(r+1)/2 <= n using binary search.
     * __int128 is used to avoid overflow in the comparison. */
    idx_t low = (idx_t)0, high = n, r = (idx_t)0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 mid_sum = (__int128)mid * (mid + (idx_t)1) / 2;
        if (mid_sum <= n) {
            r = mid;
            low = mid + (idx_t)1;
        } else {
            if (mid == (idx_t)0) break;   /* safeguard against underflow */
            high = mid - (idx_t)1;
        }
    }

    /* Compute offset of the first element in row r. */
    __int128 offset128 = (__int128)r * (r + (idx_t)1) / 2;
    idx_t offset = (idx_t)offset128;   /* offset fits in idx_t */

    idx_t k = n - offset;              /* column index within the row */

    seq_t val = binomial_multiplicative(r, k);
    val += (seq_t)r;                   /* add the row number */

    return val;
}
