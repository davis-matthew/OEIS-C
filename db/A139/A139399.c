/*
 * A139399  Number of steps to reach a cycle in Collatz problem.
 * https://oeis.org/A139399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Collatz steps to reach a cycle (n = 1, 2, 4)
 * Range:     u8 n <= 26, u16 n <= 3030, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t cur = (seq_t)n;
    seq_t steps = 0;
    while (cur != 1 && cur != 2 && cur != 4) {
        if ((cur & 1) == 0)
            cur = cur / 2;
        else
            cur = 3 * cur + 1;
        steps++;
    }
    return steps;
}
