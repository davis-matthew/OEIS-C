/*
 * A018613  Divisors of 726.
 * https://oeis.org/A018613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 726 in ascending order (0 if n > 12)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool divs_init = false;
static seq_t divs[12];

static void init_divs(void)
{
    seq_t tmp[12];
    int idx = 0;
    /* Generate all divisors 2^e2 * 3^e3 * 11^e11 with 0≤e2, e3 ≤1 and 0≤e11≤2 */
    for (int e2 = 0; e2 <= 1; ++e2) {
        seq_t v2 = (seq_t)1;
        for (int i = 0; i < e2; ++i) v2 *= 2;
        for (int e3 = 0; e3 <= 1; ++e3) {
            seq_t v3 = v2;
            for (int i = 0; i < e3; ++i) v3 *= 3;
            for (int e11 = 0; e11 <= 2; ++e11) {
                seq_t v = v3;
                for (int i = 0; i < e11; ++i) v *= 11;
                tmp[idx++] = v;
            }
        }
    }
    /* Simple insertion sort for the 12 values */
    for (int i = 1; i < 12; ++i) {
        int j = i;
        while (j > 0 && tmp[j-1] > tmp[j]) {
            seq_t t = tmp[j-1];
            tmp[j-1] = tmp[j];
            tmp[j] = t;
            --j;
        }
    }
    for (int i = 0; i < 12; ++i) divs[i] = tmp[i];
    divs_init = true;
}

seq_t a(idx_t n)
{
    if (!divs_init)
        init_divs();
    if (n == 0)               /* safety for unexpected n==0 */
        return (seq_t)0;
    if (n <= 12U)             /* valid indices */
        return divs[n-1];
    return (seq_t)0;           /* out of range: return 0 */
}
