/*
 * A180968  The only integers that cannot be partitioned into a sum of six
 *          positive squares.
 * https://oeis.org/A180968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A180968:
   a(n) = n      for 1 ≤ n ≤ 5
   a(n) = 6 + B[n-6]      for 6 ≤ n ≤ 12
   where B = {1,2,4,5,7,10,13}
   For n > 12 the sequence is undefined – we return 0. */

seq_t a(idx_t n) {
    /* B is part of the definition, not a lookup table of the sequence. */
    static const seq_t B[7] = {1, 2, 4, 5, 7, 10, 13};

    if (n <= 5) {
        return (seq_t)n;
    } else if (n <= 12) {
        idx_t idx = n - 6;          /* idx ∈ [0,6] */
        return (seq_t)6 + B[idx];
    } else {
        return (seq_t)0;            /* beyond defined terms */
    }
}
