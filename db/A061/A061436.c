/*
 * A061436  Number of steps for trajectory of n to reach 1 under the map that
 *          sends x -> x/3 if x mod 3 = 0, x -> x+3-(x mod 3) if x is not 0
 *          mod 3 (for a 2nd time when n starts at 1).
 * https://oeis.org/A061436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of steps for the trajectory of n to reach 1 under
 *            the map f(x) = x/3 if x mod 3 == 0 f(x) = x + 3 – (x mod 3)
 *            otherwise. For n = 1 the trajectory requires 2 steps (1 → 3 → 1)
 * Range:     u8 n <= 1857, u16 n <= 9859, u32 n <= 9859, u64 n <= 9859,
 *            u128 n <= 9859, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 2;                /* special rule for the starting value 1 */

    seq_t steps = 0;
    seq_t x = (seq_t)n;

    while (x != 1) {
        if ((x % 3) == 0)
            x = x / 3;
        else
            x = x + 3 - (x % 3);
        ++steps;
    }
    return steps;
}
