/*
 * A184016  n+floor(3*sqrt(n)); complement of A184017.
 * https://oeis.org/A184016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(3*sqrt(n))
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t isqrt_u64(idx_t n)
{
    /* binary search for floor(sqrt(n)) using 128‑bit multiplication to avoid overflow */
    idx_t low = 0;
    idx_t high = ((idx_t)1 << 32);          /* 2^32 is an upper bound for sqrt(64‑bit numbers) */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;                       /* low is 1 above the desired value */
}

seq_t a(idx_t n)
{
    /* floor(sqrt(n)) */
    idx_t s = isqrt_u64(n);

    /* compute floor(3*sqrt(n)) efficiently without floating point.
       Using the identities:
       let x = 9*n
       let t1 = 9*s^2 + 6*s + 1        // x < t1  => add = 0
       let t2 = 9*s^2 +12*s + 4        // t1 ≤ x < t2 => add = 1
       x ≥ t2   => add = 2
    */
    unsigned __int128 n128 = n;
    unsigned __int128 s128 = s;
    unsigned __int128 x   = n128 * 9ULL;
    unsigned __int128 t1  = s128 * s128 * 9ULL + s128 * 6ULL + 1;
    unsigned __int128 t2  = s128 * s128 * 9ULL + s128 * 12ULL + 4;

    idx_t add = 0;
    if (x >= t2)
        add = 2;
    else if (x >= t1)
        add = 1;

    /* floor(3*sqrt(n)) = 3*s + add */
    seq_t floor_term = (seq_t)(3 * s + add);

    /* a(n) = n + floor(3*sqrt(n)) */
    return (seq_t)n + floor_term;
}
