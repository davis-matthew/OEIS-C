/*
 * A021055  Decimal expansion of 1/51.
 * https://oeis.org/A021055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth digit of the decimal expansion of 1/51 (period 16:
 *            0,1,9,6,0,7,8,4,3,1,3,7,2,5,4,9)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static seq_t digits[16];
    static int initialized = 0;

    if (!initialized) {
        unsigned long long rem = 1ULL;          /* remainder mod 51 */
        for (idx_t i = 0; i < 16; ++i) {
            unsigned long long prod = rem * 10ULL;  /* multiply by 10 */
            digits[i] = (seq_t)(prod / 51ULL);      /* digit */
            rem = prod % 51ULL;                     /* new remainder */
        }
        initialized = 1;
    }

    return digits[n % 16];
}
