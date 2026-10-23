/*
 * A119281  Number of counting rods to represent n in the ancient Chinese rod
 *          numeral system.
 * https://oeis.org/A119281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = sum of rod counts for each decimal digit of n * where a
 *            digit d contributes d rods if d≤5, otherwise d‑4 rods
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t temp = n;
    seq_t rods  = 0;
    while (temp != 0) {
        uint8_t d = temp % 10;    /* digit 0‑9 (fits in a byte) */
        if (d <= 5)
            rods += (seq_t)d;    /* 0→0, 1→1, … ,5→5 */
        else
            rods += (seq_t)(d - 4); /* 6→2, 7→3, 8→4, 9→5 */
        temp /= 10;
    }
    return rods;
}
