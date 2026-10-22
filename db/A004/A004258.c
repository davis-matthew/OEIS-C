/*
 * A004258  Duplicate of A029837.
 * https://oeis.org/A004258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = ceil(log2(n))
 * Range:     i8 n <= 105, i16 n <= 105, i32 n <= 105, i64 n <= 105,
 *            i128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    
    idx_t temp = n - 1;
    seq_t result = 0;
    while (temp > 0) {
        result++;
        temp >>= 1;
    }
    return result;
}
