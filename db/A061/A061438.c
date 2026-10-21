/*
 * A061438  Number of steps for trajectory of n to reach 1 under the map that
 *          sends x -> x/13 if x mod 13 = 0, x -> 14x+13-(x mod 13) if x is
 *          not 0 mod 13 (for a 2nd time when n starts at 1).
 * https://oeis.org/A061438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of steps from n to 1 in the Collatz‑like map: x ->
 *            x/13 if x % 13 == 0, x -> 14*x + 13 - (x % 13) otherwise. The
 *            map is applied until 1 is reached (for n = 1 the cycle length to
 *            return to 1 is counted)
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t steps = 0;
    seq_t x = n;
    /* Use a do‑while so that n = 1 is processed until the
       trajectory returns to 1 after at least one step. */
    do
    {
        if ((x % 13) == 0)
        {
            x /= 13;
            ++steps;
        }
        else
        {
            seq_t k = x / 13;
            seq_t r = x % 13;
            x = 14 * k + r + 1;   /* after the two steps */
            steps += 2;
        }
    } while (x != 1 || steps == 0);
    return steps;
}
