/*
 * A047584  Numbers that are congruent to {1, 3, 5, 6, 7} mod 8.
 * https://oeis.org/A047584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (40*n - 10 + 3*(n%5) + 3*((n+1)%5) - 2*((n+2)%5) * -
 *            2*((n+3)%5) - 2*((n+4)%5)) / 25
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic when available to avoid overflow of 40*n. */
#if defined(__SIZEOF_INT128__)
    unsigned __int128 num = (unsigned __int128)40 * n - 10
        + (unsigned __int128)3 * (n % 5)
        + (unsigned __int128)3 * ((n + 1) % 5)
        - (unsigned __int128)2 * ((n + 2) % 5)
        - (unsigned __int128)2 * ((n + 3) % 5)
        - (unsigned __int128)2 * ((n + 4) % 5);
    return (seq_t)(num / 25);
#else
    seq_t num = (seq_t)40 * n - (seq_t)10
        + (seq_t)3 * (n % 5)
        + (seq_t)3 * ((n + 1) % 5)
        - (seq_t)2 * ((n + 2) % 5)
        - (seq_t)2 * ((n + 3) % 5)
        - (seq_t)2 * ((n + 4) % 5);
    return num / (seq_t)25;
#endif
}
