/*
 * A162320  Array read by antidiagonals: a(n,m) = the number of digits of m
 *          when written in base n. The top row is the number of digits for
 *          each m in base 2.
 * https://oeis.org/A162320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9560, u16 n <= 9560, u32 n <= 9560, u64 n <= 9560,
 *            u128 n <= 9560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for OEIS sequence A162320: number of digits of m in base n
   read by antidiagonals.  The matrix entry a(n_base,m) = floor(log_{n_base} m) + 1,
   with n_base ≥ 2 and m ≥ 1.  The 1‑based index n of the flattened array
   is converted to the appropriate (n_base,m) pair by walking the antidiagonals
   (sum of coordinates) in decreasing order of n_base.  This implementation
   uses only integer arithmetic and works for any idx_t/seq_t chosen by the build.
*/
seq_t a(idx_t n)
{
    /* Index 1 is the first term; for safety treat n==0 as 0 */
    if (n == 0)
        return 0;

    idx_t k = n - (idx_t)1;          /* zero‑based index in the antidiagonal order */

    /* binary search for the largest s such that s*(s+1)/2 ≤ k */
    idx_t low  = 0;
    idx_t high = 1;
    while (((unsigned __int128)high * (high + (idx_t)1)) / 2 <= k) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = (low + high + (idx_t)1) >> 1;
        if (((unsigned __int128)mid * (mid + (idx_t)1)) / 2 <= k) {
            low = mid;
        } else {
            high = mid - (idx_t)1;
        }
    }
    idx_t s = low;

    idx_t before  = ((unsigned __int128)s * (s + (idx_t)1)) / 2;;
    idx_t offset  = k - before;

    /* coordinates in the matrix */
    idx_t n_base = (s - offset) + (idx_t)2; /* base ≥ 2 */
    idx_t m      = offset + (idx_t)1;       /* position in that base */

    /* count digits of m in base n_base */
    seq_t digits = 0;
    idx_t val = m;
    idx_t base = n_base;
    while (val > (idx_t)0) {
        val /= base;
        digits += (seq_t)1;
    }
    return digits;
}
