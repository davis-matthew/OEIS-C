/*
 * A018777  Divisors of 1016.
 * https://oeis.org/A018777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 1016, a(n)=0 for n>8
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    static seq_t divs[8];
    static idx_t count = 0;
    if (count == 0) {
        idx_t pos = 0;
        for (idx_t e = 0; e <= 3; ++e) {
            seq_t v = (seq_t)(1ULL << e);          /* 2^e */
            divs[pos++] = v;                       /* 2^e * 127^0 */
            divs[pos++] = v * ((seq_t)127ULL);     /* 2^e * 127^1 */
        }
        /* simple insertion sort */
        for (idx_t i = 1; i < 8; ++i) {
            seq_t key = divs[i];
            idx_t j = i;
            while (j > 0 && divs[j-1] > key) {
                divs[j] = divs[j-1];
                --j;
            }
            divs[j] = key;
        }
        count = 8;
    }
    if (n > count) return 0;
    return divs[n-1];
}
