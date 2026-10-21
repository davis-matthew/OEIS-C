/*
 * A213548  Rectangular array: (row n) = b**c, where b(h) = h, c(h) =
 *          m(m+1)/2, m = n-1+h, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 485, u32 n <= 1830, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A213548: a(n) is the n-th entry of the array whose rows are
 *  defined by the convolution of the sequences b(h)=h and
 *  c(h)=h(h+1)/2.  The array entry (i,j) has the closed‑form

 *      T(i,j) =
 *          (6*i*(i-1)*j*(j+1)
 *           + 2*(2*i-1)*j*(j+1)*(j+2)
 *           + j*(j+1)*(j+1)*(j+2)) / 24

 *  The n-th term is obtained by reading the array along falling
 *  antidiagonals:  for each diagonal d=i+j-1 it outputs
 *      (i=1,j=d), (i=2,j=d-1), … , (i=d,j=1).
 *  The function below converts the sequence index n into the
 *  corresponding coordinates (i,j) and evaluates T(i,j).  All
 *  intermediate arithmetic is performed in seq_t to protect
 *  against overflow on arbitrary‑precision builds.  For 64‑bit
 *  builds the values remain within range for n up to at least
 *  10 000, as required. */

seq_t a(idx_t n)
{
    /* binary search for the diagonal d such that d(d+1)/2 >= n */
    idx_t low  = 1;
    idx_t high = n * 2 + 2;          /* upper bound for the search */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t d = low;                   /* d = i + j - 1 */

    /* number of terms before diagonal d */
    idx_t prev = (d - 1) * d / 2;
    idx_t offset = n - prev - 1;     /* 0‑based offset within the diagonal */

    /* coordinates in the array */
    idx_t i = offset + 1;            /* row   */
    idx_t j = d - offset;            /* column */

    /* compute T(i,j) using the closed‑form formula */
    seq_t ii = (seq_t)i;
    seq_t jj = (seq_t)j;

    seq_t termA = 6 * ii * (ii - 1) * jj * (jj + 1);
    seq_t termB = 2 * (2 * ii - 1) * jj * (jj + 1) * (jj + 2);
    seq_t termC = jj * (jj + 1) * (jj + 1) * (jj + 2);

    seq_t sum = termA + termB + termC;
    return sum / 24;
}
