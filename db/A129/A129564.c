/*
 * A129564  A129360 * A000012.
 * https://oeis.org/A129564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A129564:  a(n) = 0 iff the entry is among the leading zeros
    in its row of the lower–triangular product of A129360 with A000012.
    For row r, the number of zeros at the start equals r – φ(2r),
    where φ is Euler's totient function.  Then
    a(n) = 1 if k > r – φ(2r), 0 otherwise,
    with k = n – T_{r-1} and T_{r-1} = (r-1)r/2.  */
seq_t
a(idx_t n) {
    /* Find the row index r such that T_{r-1} < n ≤ T_r  */
    idx_t r = 1;
    idx_t tri = 1;           /* T_r  */
    while (tri < n) {
        ++r;
        tri += r;            /* T_r = r(r+1)/2  */
    }

    /* Position within the row  (1‑based)  */
    idx_t k = n - (tri - r);

    /* Compute φ(2r)  */
    idx_t m = 2 * r;
    idx_t phi = m;
    idx_t tmp = m;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            while (tmp % p == 0) tmp /= p;
            phi -= phi / p;
        }
    }
    if (tmp > 1) phi -= phi / tmp;

    /* Number of leading zeros in row r  */
    idx_t zeros = r - phi;

    /* The entry is 1 if it is past the leading zeros, else 0 */
    return (k > zeros) ? (seq_t)1 : (seq_t)0;
}
