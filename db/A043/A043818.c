/*
 * A043818  Numbers n such that number of runs in the base 3 representation of
 *          n is congruent to 3 mod 10.
 * https://oeis.org/A043818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: nth number whose base‑3 representation has a number of runs
   congruent to 3 modulo 10.  The function searches numerically. */
static inline int runs_in_base3(idx_t x)
{
    int runs = 0;
    while (x > 0) {
        int d = (int)(x % 3);      /* digit in base 3 */
        while (x % 3 == d) x /= 3; /* consume the run */
        runs++;
    }
    return runs;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but guard for safety */
    idx_t found = 0;
    for (idx_t num = 1;; ++num) {
        if (runs_in_base3(num) % 10 == 3) {
            ++found;
            if (found == n) return (seq_t)num;
        }
    }
}
