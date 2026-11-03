/*
 * A191928  Array read by antidiagonals: T(m,n) =
 *          floor(m/2)*floor((m-1)/2)*floor(n/2)*floor((n-1)/2).
 * https://oeis.org/A191928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (m,k) entry of T(m,k) =
 *            floor(m/2)*floor((m-1)/2)*floor(k/2)*floor((k-1)/2), where (m,k)
 *            are the unique non‑negative integers satisfying n = m + k choose
 *            2 + k (reading the infinite array by antidiagonals, starting
 *            with (0,0) on the 0‑th antidiagonal)
 * Range:     u8 n <= 119, u16 n <= 119, u32 n <= 119, u64 n <= 119,
 *            u128 n <= 119, big any n
 * Validated: partial, pass, 120 terms through n = 119 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t _f_part(idx_t x)
{
    /* floor(x/2) * floor((x-1)/2), avoiding unsigned underflow */
    seq_t a = (seq_t)(x >> 1);
    seq_t b = (x <= 1) ? 0 : (seq_t)(((idx_t)x - 1) >> 1);
    return a * b;
}

seq_t a(idx_t n)
{
    /* Find the antidiagonal (sum s) containing index n */
    idx_t s = 0;
    while (((s + 1) * (s + 2)) / 2 <= n)
        ++s;

    /* Offset within the antidiagonal */
    idx_t prev = (s * (s + 1)) / 2;
    idx_t t = n - prev;          /* 0‑based position on this diagonal */

    /* Coordinates on the array: (m, k) = (s - t, t) */
    idx_t m = s - t;
    idx_t k = t;

    /* Compute T(m,k) */
    seq_t fm = _f_part(m);
    seq_t fk = _f_part(k);
    return fm * fk;
}
