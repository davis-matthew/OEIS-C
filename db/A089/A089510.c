/*
 * A089510  A periodic sequence with period length 30.
 * https://oeis.org/A089510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n odd?1:0) – (n divisible by 3?1:0) – (n divisible by
 *            5?1:0) + (n divisible by 30?1:0)
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to the OEIS definition: a(n)=1‑floor(2^(⌊n/2⌋–n/2))‑floor(2^(⌊n/3⌋–n/3))‑floor(2^(⌊n/5⌋–n/5))+floor(2^(⌊n/30⌋–n/30)). */

/* Compute the n-th term of A089510 (period 30) for any n ≥ 1. */
seq_t a(idx_t n)
{
    seq_t val = 0;

    if (n & ((seq_t)1))            /* n odd → +1 */
        val += 1;
    if ((n % (idx_t)3) == 0)       /* divisible by 3 → –1 */
        val -= 1;
    if ((n % (idx_t)5) == 0)       /* divisible by 5 → –1 */
        val -= 1;
    if ((n % (idx_t)30) == 0)      /* divisible by 30 → +1 */
        val += 1;

    return val;
}
