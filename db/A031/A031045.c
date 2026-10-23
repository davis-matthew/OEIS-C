/*
 * A031045  Triangle T(n,k): write n in base 8, reverse order of digits.
 * https://oeis.org/A031045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf, less
 * Formula:   not stated
 * Range:     u8 n <= 9403, u16 n <= 9403, u32 n <= 9403, u64 n <= 9403,
 *            u128 n <= 9403, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) from OEIS A031045:  
   For the triangle T(n,k) where T(n,k) is the k‑th digit of the base‑8
   representation of n, reversed (k=0 gives the least significant digit),
   this function returns the sequence of these reversed digits read row‑major.
   The value at index n is obtained by finding the row r such that the
   cumulative row lengths up to r-1 ≤ n < cumulative length up to r,
   and then the offset k = n - cumulative length up to r-1.  The k‑th
   reversed base‑8 digit of r is returned.  */
seq_t a(idx_t n)
{
    idx_t r = 0;         /* current row number   */
    idx_t rem = n;       /* remaining index within the suffix  */

    /* Find the row r that contains the index n.  */
    while (1) {
        idx_t len = 1;   /* number of base‑8 digits in row r  */
        if (r > 0) {
            idx_t t = r;
            while (t >= 8) {   /* count digits of r in base 8 */
                t /= 8;
                len++;
            }
        }

        if (rem < len) /* n lies in this row */
            break;

        rem -= len;    /* skip this whole row   */
        r++;           /* move to the next row  */
    }

    /* rem now holds the offset k within row r (0‑based).  */
    idx_t offset = rem;
    idx_t tmp = r;
    for (idx_t i = 0; i < offset; ++i)
        tmp /= 8;      /* move to the k‑th least significant digit */

    seq_t digit = (seq_t)(tmp % 8);  /* the k‑th reversed digit */
    return digit;
}
