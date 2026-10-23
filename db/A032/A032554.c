/*
 * A032554  Arrange digits of cubes in descending order.
 * https://oeis.org/A032554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = A004186(A000578(n)) = arrange digits of n^3 in descending
 *            order
 * Range:     u8 n <= 4, u16 n <= 21, u32 n <= 1001, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n^3 */
    seq_t cube = (seq_t)n * (seq_t)n * (seq_t)n;

    /* count decimal digits of cube */
    unsigned char cnt[10] = {0};
    seq_t temp = cube;
    if (temp == 0) {
        cnt[0] = 1;
    } else {
        while (temp > 0) {
            unsigned digit = (unsigned)(temp % 10);
            cnt[digit]++;
            temp /= 10;
        }
    }

    /* rebuild the number with digits sorted descending */
    seq_t result = 0;
    for (int d = 9; d >= 0; --d) {
        while (cnt[d]) {
            result = result * 10 + d;
            cnt[d]--;
        }
    }

    return result;
}
