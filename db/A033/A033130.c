/*
 * A033130  Base-3 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,1,0.
 * https://oeis.org/A033130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(6*3^n/13)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If seq_t is small (≤ 64‑bit) we use an unsigned 128‑bit
       integer for intermediate calculations to avoid overflow. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 pow = 1;
        unsigned __int128 base = 3;
        idx_t e = n;
        while (e) {
            if (e & (idx_t)1) pow *= base;
            e >>= 1;
            if (e) base *= base;
        }
        return (seq_t)((pow * (unsigned __int128)6) / (unsigned __int128)13);
    }

    /* For larger seq_t (arbitrary‑precision or ≥128‑bit) we rely on
       seq_t arithmetic, which can handle the required size. */
    seq_t pow = 1;
    seq_t base = 3;
    idx_t e = n;
    while (e) {
        if (e & (idx_t)1) pow *= base;
        e >>= 1;
        if (e) base *= base;
    }
    return (pow * (seq_t)6) / (seq_t)13;
}
