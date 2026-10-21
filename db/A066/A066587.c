/*
 * A066587  Duplicate of A047621.
 * https://oeis.org/A066587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 8 * ⌊(n-1)/2⌋ + (n odd ? 3 : 5)
 * Range:     i8 n <= 24, i16 n <= 24, i32 n <= 24, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Numbers congruent to 3 or 5 modulo 8) */
seq_t a(idx_t n)
{
    seq_t m   = (seq_t)(n - 1) / 2;             /* floor((n-1)/2)  */
    seq_t base = (seq_t)8 * m;                  /* 8 * floor((n-1)/2) */
    seq_t add  = (n & 1) ? (seq_t)3 : (seq_t)5;/* 3 if n odd, 5 otherwise */
    return base + add;
}
