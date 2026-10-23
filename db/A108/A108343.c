/*
 * A108343  Gapful numbers >= 100: numbers that are divisible by the number
 *          formed by their first and last digit. Numbers up to 100 trivially
 *          have this property and are excluded.
 * https://oeis.org/A108343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 6311, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = the n-th gapful number ≥100.
    A number m is gapful if the two‑digit number formed from its first
    and last decimal digits divides m.  This function searches numbers
    sequentially and returns the requested term. */
seq_t a(idx_t n)
{
    /* n is 1‑based; for n=0 we return 0 (undefined). */
    if (n == 0)
        return (seq_t)0;

    seq_t x = 100;      /* first candidate */
    idx_t found = 0;    /* number of gapfuls seen so far */

    while (found < n) {
        /* last digit of x */
        seq_t last = x % 10;

        /* first digit of x: keep dividing by 10 until <10 */
        seq_t tmp = x;
        while (tmp >= 10)
            tmp = tmp / 10;
        seq_t first = tmp;

        /* two‑digit number from first and last digits */
        seq_t fd = first * 10 + last;

        /* test gapful condition */
        if (fd != 0 && (x % fd) == 0) {
            found++;
            if (found == n)
                return x;
        }
        x++;   /* next integer */
    }

    /* never reached */
    return (seq_t)0;
}
