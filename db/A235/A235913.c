/*
 * A235913  a(n) is the Manhattan distance between n^3 and (n+1)^3 in a square
 *          spiral of positive integers with 1 at the center.
 * https://oeis.org/A235913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Manhattan distance between n^3 and (n+1)^3 in the
 *            standard square spiral with 1 at the centre
 * Range:     u8 n <= 32, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Conversion of a 64‑bit unsigned integer to its integer square root
 * without using floating point. */
static uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t lo = 0, hi = 0x100000000ULL; /* sqrt(2^64-1) < 2^32 */
    while (lo < hi)
    {
        uint64_t mid = (lo + hi) >> 1;
        if (mid > 0 && mid > x / mid)  /* mid*mid > x avoiding overflow */
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo - 1;
}

/* Return the (x,y) coordinates in the Ulam spiral of the number k
 * (k >= 1).  The result is stored in the signed 64‑bit variables x,y.
 * It is safe for k up to about 1e18 (more than enough for the tests). */
static void spiral_coord(uint64_t k, long long *x, long long *y)
{
    if (k == 1) {
        *x = 0; *y = 0;
        return;
    }

    /* s = ceil(sqrt(k)) rounded up to the next odd integer. */
    uint64_t s = isqrt_uint64(k);
    if (s * s < k)
        s++;
    if ((s & 1) == 0)
        s++;

    uint64_t r = (s - 1) / 2;          /* ring index */
    uint64_t t = s * s;                /* max number of this ring   */
    uint64_t d = t - k;                /* offset from the max position */

    if (d <= 2 * r) {
        *x = (long long)r - (long long)d;
        *y = -(long long)r;
    } else if (d <= 4 * r) {
        uint64_t h = d - (2 * r + 1);
        *x = -(long long)r;
        *y = -(long long)r + 1 + (long long)h;
    } else if (d <= 6 * r) {
        uint64_t h = d - (4 * r + 1);
        *x = -(long long)r + 1 + (long long)h;
        *y = (long long)r;
    } else { /* d <= 8*r - 1 */
        uint64_t h = d - (6 * r + 1);
        *x = (long long)r;
        *y = (long long)r - 1 - (long long)h;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* offset starts at 1, but keep defined */

    uint64_t n64 = (uint64_t)n;
    uint64_t a64 = n64 * n64 * n64;
    uint64_t b64 = (n64 + 1) * (n64 + 1) * (n64 + 1);

    long long x1, y1, x2, y2;
    spiral_coord(a64, &x1, &y1);
    spiral_coord(b64, &x2, &y2);

    long long dx = x1 - x2;
    long long dy = y1 - y2;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;

    return (seq_t)(dx + dy);
}
