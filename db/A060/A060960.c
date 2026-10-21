/*
 * A060960  Duplicate of A073642.
 * https://oeis.org/A060960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = sum of the positions (0‑based) of the set bits in n.
 *            Hence a(n) = Σ_{i≥0} i·bit_i(n). The recurrence a(n) = a(n −
 *            2^⌊log2 n⌋) + ⌊log2 n⌋ gives the same. For n = 1, the value is
 *            0, for n = 2, 1, for n = 3, 1, etc
 * Range:     i8 n <= 34, i16 n <= 34, i32 n <= 34, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t pos  = 0;
    while (n != 0) {
        if (n & 1ULL) {
            sum += (seq_t)pos;
        }
        pos++;
        n >>= 1;
    }
    return sum;
}
