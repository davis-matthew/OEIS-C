/*
 * A132207  Triangle read by rows: row n lists first 2*4^n terms of an array
 *          read by rows, in which row k gives 2*4^n + 3*k - 1; 5*4^n + 3*k -
 *          1, with k>=0 in each array.
 * https://oeis.org/A132207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A132207
    Triangle read by rows:   Row n contains 2*4^n values
    row k (starting at 0) contributes two successive terms
        2*4^n + 3*k – 1   and   5*4^n + 3*k – 1
    with k=0,…,4^n−1.
    The sequence a(n) is the linear unfolding of these rows.

    Algorithm:
    1. Find the row m so that n lies in the interval
           offset(m) ≤ n < offset(m)+2*4^m
       where offset(m)=Σ_{i=0}^{m-1} 2*4^i = 2*(4^m−1)/3.
    2. Compute p = n – offset(m)
    3. k = p / 2 ,  pos = p % 2
    4. If pos==0:  a(n)=2*4^m + 3*k –1
       else:        a(n)=5*4^m + 3*k –1
*/
seq_t a(idx_t n) {
    idx_t pow4 = 1;              /* 4^m  */
    idx_t block = 2;             /* 2*4^m per row */
    idx_t offset = 0;            /* start index of current row */

    /* Find the row m where n resides */
    while (n >= offset + block) {
        offset += block;
        pow4 *= 4;
        block = 2 * pow4;
    }

    /* Relative position inside the found row */
    idx_t p = n - offset;
    idx_t k = p >> 1;  /* integer division by 2 */
    idx_t pos = p & 1; /* 0 or 1 */

    /* Compute the term */
    seq_t base = (seq_t)pow4;
    seq_t term1 = (seq_t)2 * base + (seq_t)3 * (seq_t)k - (seq_t)1;
    seq_t term2 = (seq_t)5 * base + (seq_t)3 * (seq_t)k - (seq_t)1;

    return pos ? term2 : term1;
}
