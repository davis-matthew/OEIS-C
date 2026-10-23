/*
 * A018595  Divisors of 692.
 * https://oeis.org/A018595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 692 in ascending order, a(1)=1,
 *            a(2)=2, a(3)=4, a(4)=173, a(5)=346, a(6)=692
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 692 = 2^2 * 173, it has exactly 6 divisors */
    static seq_t divisors[15];
    static size_t dcnt = 0;
    static bool initialized = false;
    static const idx_t N = 692;   /* constant number */

    if (!initialized) {
        /* Find all divisors */
        for (idx_t i = 1; (seq_t)i * (seq_t)i <= (seq_t)N; ++i) {
            if (N % i == 0) {
                divisors[dcnt++] = (seq_t)i;
                idx_t j = N / i;
                if (j != i) {
                    divisors[dcnt++] = (seq_t)j;
                }
            }
        }
        /* Simple insertion sort to order ascending */
        for (size_t i = 1; i < dcnt; ++i) {
            seq_t key = divisors[i];
            size_t j = i;
            while (j > 0 && divisors[j - 1] > key) {
                divisors[j] = divisors[j - 1];
                --j;
            }
            divisors[j] = key;
        }
        initialized = true;
    }

    if (n == 0 || n > (idx_t)dcnt) {
        /* Undefined behaviour for n outside 1..6; return 0 */
        return (seq_t)0;
    }

    return divisors[n - 1];
}
