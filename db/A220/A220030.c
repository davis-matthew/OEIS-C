/*
 * A220030  Number of n X 6 arrays of the minimum value of corresponding
 *          elements and their horizontal or diagonal neighbors in a random,
 *          but sorted with lexicographically nondecreasing rows and
 *          nonincreasing columns, 0..1 n X 6 array.
 * https://oeis.org/A220030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A220030
 *
 * Recurrence (for n > 7):
 *   a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) - a(n-6)
 *
 * Initial values:
 *   a(1)=6, a(2)=15, a(3)=42, a(4)=95, a(5)=192, a(6)=358, a(7)=626
 *
 * The recurrence follows from the generating function
 *   x*(3-6x+6x^2-2x^3)*(2-3x+4x^2-2x^3) / (1-x)^6
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n=0, return 0 for safety */

    /* first 7 terms */
    static const seq_t first[] = {6, 15, 42, 95, 192, 358, 626};
    if (n <= 7) return first[n-1];

    /* initialise last six values a(n-1) … a(n-6) for n=8 */
    seq_t prev[6];
    prev[0] = first[6]; /* a(7)  */
    prev[1] = first[5]; /* a(6)  */
    prev[2] = first[4]; /* a(5)  */
    prev[3] = first[3]; /* a(4)  */
    prev[4] = first[2]; /* a(3)  */
    prev[5] = first[1]; /* a(2)  */

    /* iterate from n=8 up to desired n */
    for (idx_t i = 8; i <= n; ++i) {
        seq_t next =
            6 * prev[0]
          - 15 * prev[1]
          + 20 * prev[2]
          - 15 * prev[3]
          + 6 * prev[4]
          - prev[5];

        /* shift window: drop a(n-6) and insert new a(n) */
        prev[5] = prev[4];
        prev[4] = prev[3];
        prev[3] = prev[2];
        prev[2] = prev[1];
        prev[1] = prev[0];
        prev[0] = next;
    }

    return prev[0];  /* a(n) */
}
