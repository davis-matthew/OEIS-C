/*
 * A006577  Number of halving and tripling steps to reach 1 in '3x+1' problem,
 *          or -1 if 1 is never reached.
 * https://oeis.org/A006577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, hear, look
 * Formula:   a(n) = number of steps in Collatz sequence (3x+1 problem) until
 *            reaching 1
 * Range:     u8 n <= 26, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t steps = 0;
    
    while (x != 1) {
        if (x % 2 == 0) {
            x = x / 2;
        } else {
            x = 3 * x + 1;
        }
        steps++;
    }
    
    return steps;
}
