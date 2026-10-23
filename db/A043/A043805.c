/*
 * A043805  Numbers n such that number of runs in the base 3 representation of
 *          n is congruent to 7 mod 8.
 * https://oeis.org/A043805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043805: numbers n such that the number of runs in the base‑3
 * representation of n is congruent to 7 modulo 8.
 * The term a(n) for n ≥ 1 is found by brute‑force searching the positive
 * integers until the nth qualifying value is located.  No external
 * dependencies are required.  The implementation works for the
 * built‑in integer types (uint8_t … uint64_t, unsigned __int128) as
 * well as for arbitrary‑precision integer classes that can be
 * constructed from an integral value (e.g. oeis_big). */

static int runs_in_base3(idx_t x)
{
    /* Count transitions in the base‑3 digits of x.  For x == 0 the
     * representation is “0”, which has one run. */
    if (x == 0)
        return 1;
    int runs = 0;
    int prev = -1;
    while (x)
    {
        int d = x % 3;
        x /= 3;
        if (d != prev)
        {
            ++runs;
            prev = d;
        }
    }
    return runs;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; behaviour for n==0 is irrelevant */
        return (seq_t)0;

    idx_t count = 0;     /* how many qualifying numbers have been found */
    idx_t value = 0;     /* candidate value to test */

    while (1)
    {
        ++value;
        if ( (runs_in_base3(value) & 7) == 7 )   /* runs % 8 == 7 */
        {
            ++count;
            if (count == n)
                return (seq_t)value;  /* cast to the output type */
        }
    }
}
