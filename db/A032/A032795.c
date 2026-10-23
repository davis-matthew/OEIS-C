/*
 * A032795  Positive numbers k such that
 *          (k+1)*(k+2)*(k+3)*(k+4)/(k+(k+1)+(k+2)+(k+3)+(k+4)) is an integer.
 * https://oeis.org/A032795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th value of ((k+1)(k+3)(k+4))/5 where k satisfies
 *            k mod 5 ∈ {1,2,4}. Effectively, a(n) is the n-th term of the
 *            sequence of integers (k+1)(k+3)(k+4)/5 with k in {1,2,4} mod 5
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, return 0 for safety */
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        idx_t r = k % 5;
        if (r == 1 || r == 2 || r == 4) {
            ++count;
            if (count == n) {
                seq_t k1 = (seq_t)(k + 1);
                seq_t k3 = (seq_t)(k + 3);
                seq_t k4 = (seq_t)(k + 4);
                return (k1 * k3 * k4) / 5;
            }
        }
    }
    /* Not reachable. */
    return 0;
}
