/*
 * A157319  Possible total points for a single team in a game of American
 *          football, ignoring safeties (and time constraints).
 * https://oeis.org/A157319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n=1, 3 if n=2, 6 if n=3, 7 if n=4, 9 if n=5, 10 if
 *            n=6; for n≥7, a(n) = n + 5 (all numbers ≥12 are reachable)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t base[] = {0, 3, 6, 7, 9, 10};
    if (n <= 6)
        return base[n - 1];
    return (seq_t)n + 5;
}
