/*
 * A216754  Digital root of fourth power of Fibonacci numbers.
 * https://oeis.org/A216754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = digital root of (Fibonacci(n)^4) Uses Pisano period 24
 *            modulo 9 to compute the desired value efficiently
 * Range:     u8 n <= 108, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base case: F(0) = 0 → 0^4 = 0, digital root = 0. */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Fibonacci numbers modulo 9 repeat with period 24. */
    static const unsigned int fib_mod9[24] = {
        0,1,1,2,3,5,8,4,
        3,7,1,8,0,8,8,7,
        6,4,1,5,6,2,8,1
    };

    /* Retrieve F(n) mod 9. */
    unsigned int d = fib_mod9[n % 24ULL];

    /* Mapping from d to the digital root of d^4 (d in 0..8). */
    static const unsigned int dr_from_d[9] = {
        9,1,7,9,4,4,9,7,1
    };

    return (seq_t)dr_from_d[d];
}
