/*
 * A066181  Permutation of the integers with cycle form {1}, {2, 3}, {4, 5,
 *          6}, {7, 8, 9, 10}, ...
 * https://oeis.org/A066181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A066181: Permutation of the integers with cycle form {1}, {2,3}, {4,5,6}, {7,8,9,10}, … 
 * This implementation finds, for a given n (n≥1), the block of size k containing n,
 * where k is minimal with k·(k+1)/2 ≥ n, and then rotates that block left by one.
 * The routine uses only integer arithmetic; an auxiliary integer‑square‑root
 * routine is supplied for 64‑bit indices.
 */

static inline idx_t isqrt_u64(idx_t x)
/* return floor(sqrt(x)) for 0 ≤ x ≤ 2⁶⁴–1 */
{
    idx_t r = 0;
    idx_t bit = 1ULL << 62;             /* largest power of 4 ≤ 2⁶⁴ */
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else
            r >>= 1;
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, but harmless */

    /* k = ceil( (sqrt(8*n+1) – 1) / 2 )  →  the block size containing n */
    idx_t x   = 8 * n + 1;
    idx_t r   = isqrt_u64(x);
    idx_t k   = (r - 1) / 2;          /* candidate k */
    idx_t tmax = k * (k + 1) / 2;     /* T(k) */
    if (tmax < n)                    /* adjust if necessary */
        ++k;

    /* Start index of the current block (T(k‑1)+1) */
    idx_t tPrev = (k - 1) * k / 2;    /* T(k‑1) */
    idx_t start = tPrev + 1;

    /* Offset of n within the block */
    idx_t offset = n - start;

    /* Rotate left by one: if not the last element, output n+1; else output start */
    if (offset < (k - 1))
        return (seq_t)(n + 1);
    else
        return (seq_t)start;
}
