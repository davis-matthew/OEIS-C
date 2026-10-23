/*
 * A126792  Removing the first, fourth, seventh, tenth ... term of the
 *          sequence yields the original sequence, augmented by 1.
 * https://oeis.org/A126792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:
 *   a(0) = 0
 *   For n ≥ 1:
 *     if n % 3 == 0:   a(n) = a(n / 3)
 *     if n % 3 == 1:   a(n) = a( 2 * (n / 3) ) + 1
 *     if n % 3 == 2:   a(n) = a( 2 * (n / 3) + 1 ) + 1
 *   (integer division is used)
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t q = n / 3;
    switch (n % 3) {
    case 0:
        return a(q);
    case 1:
        return a(2 * q) + 1;
    case 2:
        return a(2 * q + 1) + 1;
    default:
        return 0; /* unreachable, but keeps the compiler happy */
    }
}
