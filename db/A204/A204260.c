/*
 * A204260  Symmetric matrix given by f(i,j)=ceiling(i*j/(i+j)) .
 * https://oeis.org/A204260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = f(i,j) where f(i,j)=ceil(i*j/(i+j)) and (i,j) are * the
 *            pair obtained by reading the symmetric matrix f(i,j) along its *
 *            anti‑diagonals (constant i+j), with i increasing. This
 *            enumerates * all entries in the matrix in the order used by OEIS
 *            A204260
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t isqrt_128(__int128 x)
/* floor(sqrt(x)) for 0 <= x <= 2^127-1 */
{
    idx_t lo = 0, hi = (idx_t)(-1);          /* maximum idx_t */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 mid2 = (__int128)mid * mid;
        if (mid2 == x) return mid;
        else if (mid2 < x) lo = mid + 1;
        else hi = mid - 1;
    }
    return hi;                               /* floor */
}

seq_t a(idx_t n)
/* a(1) is the first term of the sequence (offset 1). */
{
    if (n == 0) return 0;                    /* undefined for n=0 */
    /* Find the anti‑diagonal index s = i+j.  The number of terms
     * up to diagonal s is C(s) = (s-1)*s/2.  We need the smallest
     * s with C(s) >= n. */
    __int128 d = (__int128)n * 8 + 1;         /* 8*n + 1 */
    idx_t raiz = isqrt_128(d);
    idx_t s = (1 + raiz) >> 1;                /* floor((1+sqrt)/2) */
    while (((__int128)(s - 1) * s) / 2 < n) ++s;
    /* Number of terms up to the previous diagonal */
    __int128 prev = ((__int128)(s - 2) * (s - 1)) / 2;
    idx_t offset = n - (idx_t)prev - 1;       /* 0‑based offset on the diagonal */
    idx_t i = 1 + offset;
    idx_t j = s - i;                          /* c = s - i */
    /* Compute ceil(i*j/(i+j)) */
    seq_t prod  = (seq_t)i * (seq_t)j;
    seq_t denom = (seq_t)(i + j);
    seq_t val = prod / denom;
    if (prod % denom != 0) ++val;
    return val;
}
