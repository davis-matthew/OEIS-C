/*
 * A228137  Numbers that are congruent to {1, 4} mod 12.
 * https://oeis.org/A228137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -13/2 - 3*(-1)^n/2 + 6*n
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: if n is odd  a(n) = 6*n - 5; if n is even a(n) = 6*n - 8 */

seq_t a(idx_t n)
{
    seq_t N = n;            /* convert index to seq_t to avoid mixed‐type issues */
    seq_t six = (seq_t)6;   /* use an explicit seq_t integer for safety */

    if (n & 1ULL) {           /* odd n */
        return six * N - (seq_t)5;
    } else {                  /* even n */
        return six * N - (seq_t)8;
    }
}
