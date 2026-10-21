/*
 * A060464  Numbers that are not congruent to 4 or 5 mod 9.
 * https://oeis.org/A060464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = base[(n-1) % 7] + 9 * floor((n-1) / 7) with base = {0, 1,
 *            2, 3, 6, 7, 8}. This gives A060464, the numbers not congruent to
 *            4 or 5 modulo 9. The definition is equivalent to the recurrence
 *            a(n+7) = a(n) + 9 for n ≥ 1, with the initial values a(1)=0, …,
 *            a(7)=8
 * Range:     u8 n <= 200, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t base[7] = {0, 1, 2, 3, 6, 7, 8};

    if (n <= 7)
        return base[n - 1];

    idx_t q = (n - 1) / 7;            // number of complete blocks of 7
    int r   = (int)((n - 1) % 7);    // remainder within the block

    return base[r] + (seq_t)9 * q;
}
