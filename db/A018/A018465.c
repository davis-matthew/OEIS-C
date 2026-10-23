/*
 * A018465  Divisors of 456.
 * https://oeis.org/A018465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 456 in increasing order
 * Range:     u8 n <= 14, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static seq_t divs[16];
    static idx_t cnt = 0;
    static int ready = 0;
    if (!ready) {
        for (idx_t d = 1; d <= 456; ++d) {
            if ((456 % d) == 0)
                divs[cnt++] = (seq_t)d;
        }
        /* simple insertion sort of at most 16 elements */
        for (int i = 1; i < (int)cnt; ++i) {
            seq_t key = divs[i];
            int j = i - 1;
            while (j >= 0 && divs[j] > key) {
                divs[j + 1] = divs[j];
                --j;
            }
            divs[j + 1] = key;
        }
        ready = 1;
    }
    if (n > cnt) return (seq_t)0;
    return divs[(int)(n - 1)];
}
