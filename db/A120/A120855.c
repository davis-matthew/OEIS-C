/*
 * A120855  Row sums of triangle A120854, which is the matrix log of triangle
 *          A117939.
 * https://oeis.org/A120855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A062756(n) + A081603(n) where A062756(n) is the number
 *            of 1's in the base‑3 expansion of n, and A081603(n) is the
 *            number of 2's in the base‑3 expansion of n
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t nn = n;
    idx_t count1 = 0;
    idx_t count2 = 0;
    while (nn > 0) {
        idx_t digit = nn % 3;
        if (digit == 1)
            ++count1;
        else if (digit == 2)
            ++count2;
        nn /= 3;
    }
    return 2 * (seq_t)count1 + (seq_t)count2;
}
