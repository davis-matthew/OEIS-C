/*
 * A086460  Square array read by antidiagonals: A(n, k) = n*k + 0^k.
 * https://oeis.org/A086460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = A(n, k) read by antidiagonals of the square array A(n, k)
 *            = n*k + 0^k, where 0^0 = 1 and 0^k = 0 for k > 0
 * Range:     u8 n <= 543, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  sequence element a(n) is the n-th entry of the array A(n,k)
    read by anti‑diagonals: (0,0), (1,0),(0,1), (2,0),(1,1),(0,2), …
    If k=0 the element is 1; otherwise it is n*k.  */

seq_t a(idx_t n)
{
    /* Find diagonal index s so that
           T(s-1) = s*(s+1)/2 ≤ n < T(s) = (s+1)*(s+2)/2   */
    idx_t s = 0;
    for (;; ++s) {
        idx_t next = (s + 1) * (s + 2) / 2;
        if (next > n)
            break;
    }

    /* Offset within this diagonal: 0 … s   */
    idx_t tPrev = s * (s + 1) / 2;   /* elements before diagonal s   */
    idx_t d     = n - tPrev;         /* column index (k)   */
    idx_t i     = s - d;             /* row index (n)      */

    /* A(n, k) = n*k + 0^k   (0^0 = 1)   */
    if (d == 0)        /* k == 0 => 0^0 = 1 */
        return (seq_t)1;
    else
        return (seq_t)i * (seq_t)d;
}
