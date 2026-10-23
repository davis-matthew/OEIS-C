/*
 * A018440  Divisors of 412.
 * https://oeis.org/A018440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sorted divisors of the fixed integer 412.
   The list is generated once and cached; a(1)=1, a(2)=2, a(3)=4,
   a(4)=103, a(5)=206, a(6)=412.  For n>6 the sequence is undefined,
   so the function returns 0. */
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t divs[128];
    static size_t count = 0;
    static int initialized = 0;

    if (!initialized) {
        initialized = 1;
        const idx_t N = 412;
        /* Collect divisors */
        for (idx_t i = 1; i * i <= N; ++i) {
            if (N % i == 0) {
                divs[count++] = (seq_t)i;
                if (i * i != N)
                    divs[count++] = (seq_t)(N / i);
            }
        }
        /* Simple insertion sort to keep ascending order */
        for (size_t i = 0; i < count; ++i) {
            for (size_t j = i + 1; j < count; ++j) {
                if (divs[i] > divs[j]) {
                    seq_t tmp = divs[i];
                    divs[i] = divs[j];
                    divs[j] = tmp;
                }
            }
        }
    }

    if (n == 0 || n > count)
        return (seq_t)0;
    return divs[n - 1];
}
