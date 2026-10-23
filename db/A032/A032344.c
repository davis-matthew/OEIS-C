/*
 * A032344  Duplicate of A023764.
 * https://oeis.org/A032344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 34, i16 n <= 34, i32 n <= 34, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(1..10)=0,1,2,…,9.
   For k ≥ 1 the k‑th block contains k+2 consecutive integers
   starting at 8k+8.  The sequence is the concatenation of these
   blocks. */
seq_t a(idx_t n)
{
    if (n <= 10) {
        return (seq_t)n - 1;
    }

    /* Index into the sequence of blocks (after the initial 10 terms). */
    idx_t m = n - 10;

    /* Find the smallest k such that k*(k+5)/2 ≥ m. */
    idx_t k = 1;
    while ((k * (k + 5)) / 2 < m) {
        ++k;
    }

    /* Sum of block lengths before block k. */
    idx_t prevSum = (k - 1) * (k + 4) / 2;

    /* Position within block k (0‑based). */
    idx_t offset = m - prevSum - 1;

    /* Starting value of block k: 8k+8. */
    idx_t val = 8 * k + 8 + offset;

    return (seq_t)val;
}
