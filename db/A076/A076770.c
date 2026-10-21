/*
 * A076770  Even numbers representable as the sum of two odd composites.
 * https://oeis.org/A076770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A076770: even numbers representable as the sum of two odd composites.
   For n >= 7 the sequence follows a simple linear rule:
       a(n) = 2*n + 28
   The first six terms are special and must be returned explicitly. */

seq_t a(idx_t n) {
    /* First six terms: 18, 24, 30, 34, 36, 40 */
    static const seq_t first[6] = {18, 24, 30, 34, 36, 40};
    if (n <= 6) {
        return first[n - 1];
    }
    /* For n >= 7: a(n) = 2*n + 28 */
    return ((seq_t)n * 2) + 28;
}
