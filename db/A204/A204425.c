/*
 * A204425  Infinite matrix: f(i,j)=(2i+j+1 mod 3), by antidiagonals.
 * https://oeis.org/A204425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) equals (2*i + j + 1) mod 3 where (i,j) are the *
 *            coordinates of the n‑th entry in the antidiagonal enumeration *
 *            of the infinite matrix with f(i,j) = (2*i + j + 1) mod 3
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset (1‑based).  For n == 0 the definition
     * does not specify a value; return 0 here. */
    if (n == 0)
        return 0;

    /* Find the antidiagonal k such that the cumulative number
     * of entries up to antidiagonal k is at least n.  The k-th
     * antidiagonal (with i+j = k) contains k-1 entries.  The
     * cumulative count up to k is (k-1)*k/2.  We increment
     * k until this count reaches or exceeds n. */
    idx_t k = 2;        /* first antidiagonal has i=j=1 */
    idx_t tot = 1;      /* total entries up to current k */

    while (tot < n) {
        ++k;
        tot += k - 1;
    }

    /* Number of entries preceding antidiagonal k. */
    idx_t before = (k - 2) * (k - 1) / 2;

    /* Index (1‑based) within this antidiagonal. */
    idx_t t = n - before;

    /* Convert to matrix coordinates (i,j).  For a fixed k,
     * i runs from 1 to k-1, and j = k - i. */
    idx_t i = t;
    idx_t j = k - i;

    /* Compute the matrix value: (2*i + j + 1) mod 3. */
    seq_t ii = (seq_t)i;
    seq_t jj = (seq_t)j;
    seq_t val = (( (seq_t)2 * ii + jj + (seq_t)1 ) % (seq_t)3);

    return val;
}
