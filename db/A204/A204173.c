/*
 * A204173  Symmetric matrix based on f(i,j)=(2i-1 if max(i,j) is odd, and 0
 *          otherwise), by antidiagonals.
 * https://oeis.org/A204173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (max(i,j)+1)/2 if max(i,j) is odd, a(n) = 0 otherwise.
 *            (i,j) are the coordinates of the n‑th entry in the antidiagonal
 *            enumeration of the infinite matrix with f(i,j) = (max(i,j)+1)/2
 *            when max(i,j) is odd
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1ULL) /* undefined, return 0 for safety */
        return 0;

    /* find the antidiagonal number s such that
       n <= s*(s-1)/2  and  n > (s-1)*(s-2)/2   (term counts). */
    idx_t s = 2;          /* minimal diagonal number (i+j) */
    idx_t count = 1;      /* count of terms up to current diagonal */

    while (count < n) {
        ++s;
        count += s - 1;   /* add the size of the new diagonal */
    }

    /* previous diagonal total */
    idx_t prev = count - (s - 1);
    /* zero‑based offset inside diagonal s */
    idx_t off = n - prev - 1;

    /* coordinates in diagonal s: i runs from 1 to s-1 */
    idx_t i = off + 1;
    idx_t j = s - i;

    idx_t m = (i > j) ? i : j;   /* max(i,j) */
    if ((m & 1ULL) == 0ULL)      /* even -> zero entry */
        return 0;

    /* odd max: value = (max(i,j)+1)/2 */
    return (seq_t)((m + 1ULL) / 2ULL);
}
