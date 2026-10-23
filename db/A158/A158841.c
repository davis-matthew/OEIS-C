/*
 * A158841  Triangle read by rows, matrix product of A145677 * A004736.
 * https://oeis.org/A158841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   For 1‑indexed n let r be the row such that (r-1)*r/2 < n ≤
 *            r*(r+1)/2. Define k = n - (r-1)*r/2 (the position within row r,
 *            1‑based). Then the triangle entry is a(n) = (r-1)*r + 1 if k ==
 *            1 a(n) = (r - k + 1)*(r-1) otherwise The sequence below is the
 *            row‑wise reading of this triangle
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for idx_t and seq_t types defined by the build */

seq_t a(idx_t n) {
    if (n < 1) return 0;            /* undefined for n < 1, but guard against overflow */

    /* Find the smallest r with r*(r+1)/2 >= n.
       Since n ≤ 10⁴ in the tests, a simple loop is fast enough. */
    idx_t r = 1, cum = 1;           /* cum = r*(r+1)/2 */
    while (cum < n) {
        r++;
        cum += r;                   /* cum now equals r*(r+1)/2 */
    }

    idx_t k = n - (cum - r);        /* cum - r = (r-1)*r/2 */

    if (k == 1) {
        /* a(n) = r*(r-1) + 1 */
        return (seq_t)r * (seq_t)(r - 1) + 1;
    } else {
        /* a(n) = (r - k + 1) * (r - 1) */
        return (seq_t)(r - k + 1) * (seq_t)(r - 1);
    }
}
