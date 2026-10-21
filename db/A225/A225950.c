/*
 * A225950  Triangle for odd legs of primitive Pythagorean triangles.
 * https://oeis.org/A225950
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A225950.
   The sequence is produced by traversing the triangle
   a(n,m) = n² – m²   (n > m ≥ 1, gcd(n,m)=1, n+m odd)
   and a(n,m) = 0 otherwise, in row‑major order
   (n increasing, then m increasing).  The offset is 2:
   a(2) is the first non‑zero entry (3).  The function
   returns the value of the n‑th term (for n ≥ 2). */

#include <stdint.h>

/* Euclidean gcd for idx_t values (works in C and C++). */
static inline idx_t egcd(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t idx)
{
    /* a(2) is the first term */
    if (idx < 2) {
        return 0;
    }

    /* zero‑based position in the row‑major list */
    idx_t pos = idx - 2;

    /* Find the row (n) that contains this position */
    idx_t n_row = 2;
    idx_t cumulative = 1;          /* elements up to current row n_row */
    while (cumulative <= pos) {
        ++n_row;
        cumulative += (n_row - 1); /* add length of new row */
    }

    /* Compute the position within the found row */
    idx_t prev_cum = cumulative - (n_row - 1);
    idx_t pos_in_row = pos - prev_cum;
    idx_t m = pos_in_row + 1;      /* m starts from 1 */

    /* Apply the conditions for a primitive Pythagorean triple */
    if (((n_row + m) & 1u) != 1u || egcd(n_row, m) != 1) {
        return 0;
    }

    /* Compute the odd leg n² – m² */
    seq_t nn = (seq_t)n_row;
    seq_t mm = (seq_t)m;
    return nn * nn - mm * mm;
}
