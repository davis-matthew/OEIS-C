/*
 * A010052  Characteristic function of squares: a(n) = 1 if n is a square,
 *          otherwise 0.
 * https://oeis.org/A010052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, mult
 * Formula:   a(n) = 1 if n is a perfect square, otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a bit‑wise binary search to compute the floor of sqrt(n). */

static idx_t floor_sqrt_u64(idx_t n) {
    /* Handle the trivial cases directly. */
    if (n == 0) return 0;
    if (n == 1) return 1;

    idx_t low = 1;
    idx_t high = n >> 1;   /* sqrt(n) will never exceed n/2 for n>=2 */
    idx_t result = 0;

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;

        if (sq == n) {
            return mid;          /* exact hit */
        } else if (sq < n) {
            result = mid;
            low = mid + 1;       /* search higher half */
        } else {
            high = mid - 1;      /* search lower half */
        }
    }
    return result;
}

seq_t a(idx_t n) {
    /* a(0) is defined to be 1. */
    if (n == 0) {
        return (seq_t)1;
    }

    idx_t s = floor_sqrt_u64(n);
    /* Check whether s*s equals n; use 128‑bit to avoid overflow. */
    unsigned __int128 sq = (unsigned __int128)s * s;
    return (seq_t)((sq == n) ? 1 : 0);
}
