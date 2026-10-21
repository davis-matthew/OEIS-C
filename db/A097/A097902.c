/*
 * A097902  Multiplication table of the digits of Pi read by antidiagonals.
 * https://oeis.org/A097902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A097902:  a(n) = product of digits of Pi at positions
   (i, j) where (i, j) are obtained by reading the infinite
   multiplication table of the digits of Pi along antidiagonals.
   The diagonal index s for a given n satisfies
          s*(s+1)/2  ≤ n  <  (s+1)*(s+2)/2
   and offset = n - s*(s+1)/2 gives i; then j = s - i. */

seq_t a(idx_t n)
{
    /* 200 decimal digits of π (starting with the leading 3). */
    static const char *pi_digits =
        "314159265358979323846264338327950288419716939937510582097494459230781640625862089986280348253421170679"
        "821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819"
        "644288109756659334461284756482337867831652712019091459660277804006149048492245822006907186728941979"
        "1077741910921072199148084596200380";

    /* Find diagonal number s such that S(s-1) ≤ n < S(s),
       where S(k) = (k+1)(k+2)/2 is the triangular number. */
    idx_t s = 0;
    for (;; ++s) {
        idx_t next = (s + 1ULL) * (s + 2ULL) / 2ULL;
        if (n < next)
            break;
    }

    /* offset within the diagonal */
    idx_t base   = s * (s + 1ULL) / 2ULL;          /* S(s-1) */
    idx_t offset = n - base;                       /* 0 ≤ offset ≤ s */

    idx_t i = offset;          /* row index in the table */
    idx_t j = s - offset;      /* column index in the table */

    /* Convert digit characters to numeric values */
    unsigned char di = (unsigned char)(pi_digits[i] - '0');
    unsigned char dj = (unsigned char)(pi_digits[j] - '0');

    /* Return product of the two digits */
    return (seq_t)di * (seq_t)dj;
}
