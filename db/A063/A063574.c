/*
 * A063574  Number of steps to reach an integer == 1 (mod 4) when iterating
 *          the map n -> 3n/2 if n even or (3n+1)/2 if n odd.
 * https://oeis.org/A063574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = number of steps to reach an integer ≡ 1 (mod 4) when
 *            iterating the map n → 3n/2 if n even, and n → (3n+1)/2 if n odd
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    unsigned __int128 cur = n;      /* current value, may exceed 64 bits */
    seq_t steps = 0;

    /* iterate until cur ≡ 1 (mod 4) */
    while ((cur & 3) != 1) {
        if ((cur & 1) == 0) {
            /* even case: cur = 3*cur/2  → (cur/2)*3 */
            cur = (cur >> 1) * 3;
        } else {
            /* odd case: cur = (3*cur+1)/2  → (cur/2)*3 + 2 */
            cur = (cur >> 1) * 3 + 2;
        }
        ++steps;
    }
    return steps;
}
