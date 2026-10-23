/*
 * A018360  Divisors of 260.
 * https://oeis.org/A018360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 260 (sorted ascending)
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static void init_divisors(seq_t *arr, size_t *cnt) {
    /* 260 = 2^2 * 5 * 13 */
    const idx_t pf[3] = {2, 5, 13};
    const int   exp[3] = {2, 1, 1};

    size_t c = 0;
    for (int e0 = 0; e0 <= exp[0]; ++e0) {
        seq_t val0 = 1;
        for (int i = 0; i < e0; ++i) val0 *= pf[0];

        for (int e1 = 0; e1 <= exp[1]; ++e1) {
            seq_t val1 = val0;
            for (int i = 0; i < e1; ++i) val1 *= pf[1];

            for (int e2 = 0; e2 <= exp[2]; ++e2) {
                seq_t val = val1;
                for (int i = 0; i < e2; ++i) val *= pf[2];
                arr[c++] = val;
            }
        }
    }
    *cnt = c;

    /* simple insertion sort */
    for (size_t i = 1; i < c; ++i) {
        seq_t key = arr[i];
        size_t j = i;
        while (j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }
}

seq_t a(idx_t n) {
    static seq_t divisors[12];
    static int   initialized = 0;

    if (!initialized) {
        size_t cnt;
        init_divisors(divisors, &cnt);
        initialized = 1;
    }

    if (n == 0) return 0;                     /* n is expected ≥ 1 */
    if ((idx_t)(sizeof(divisors) / sizeof(divisors[0])) >= n)
        return divisors[n - 1];

    return 0;    /* for n beyond the list of divisors */
}
