/*
 * A043565  Number of runs in base-13 representation of n.
 * https://oeis.org/A043565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of runs in base‑13 representation of n (a(0) = 1)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The number zero has a single digit "0", thus one run. */
    if (n == 0ull)
        return (seq_t)1;

    /* Convert to base‑13, counting consecutive equal digits. */
    idx_t x = n;
    unsigned prev = (unsigned)(x % 13ull);
    x /= 13ull;

    seq_t runs = (seq_t)1;   /* first digit starts a run */

    while (x != 0ull)
    {
        unsigned curr = (unsigned)(x % 13ull);
        if (curr != prev)
        {
            runs += (seq_t)1;
            prev = curr;
        }
        x /= 13ull;
    }

    return runs;
}
