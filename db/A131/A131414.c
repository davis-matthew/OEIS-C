/*
 * A131414  A130302 + A130303 - A000012.
 * https://oeis.org/A131414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = value of the lower triangular matrix A130302 + A130303 -
 *            A000012 in row‑major order. For row r (r ≥ 1) and column c (1 ≤
 *            c ≤ r) the entry is - if c == 1: (r^2 + 5r – 4) / 2 (left column
 *            A052905) - else: 2r – (2c – 1) (the remaining entries) The term
 *            a(n) is obtained by finding the row r th...
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that T_{r-1} < n ≤ T_r where T_r = r(r+1)/2.
       Since n is small (< ~10⁴ in tests) a simple loop is fast enough and
       avoids needing an integer‑square‑root routine. */
    idx_t r = 1;
    idx_t cum = 1;                 /* T_r */
    while (n > cum) {
        ++r;
        cum += r;
    }

    /* Column index: c = n – T_{r-1}  (1‑based).  T_{r-1} = (r-1)r/2. */
    idx_t prev = (r - 1) * r / 2;
    idx_t c = n - prev;            /* 1 … r */

    if (c == 1) {
        /* Left column: (r^2 + 5r – 4) / 2.  All operations are performed
           in seq_t to handle arbitrary precision. */
        seq_t rr = (seq_t)r;
        return (rr * rr + (seq_t)5 * rr - (seq_t)4) / (seq_t)2;
    } else {
        /* Other entries: 2r – (2c – 1). */
        seq_t rr = (seq_t)r;
        seq_t cc = (seq_t)c;
        return (seq_t)2 * rr - ((seq_t)2 * cc - (seq_t)1);
    }
}
