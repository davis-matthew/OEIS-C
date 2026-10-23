/*
 * A018295  Divisors of 126.
 * https://oeis.org/A018295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of divisors of 126, sorted ascending */
/* a(1)=1, a(2)=2, a(3)=3, a(4)=6, a(5)=7, a(6)=9, a(7)=14,
   a(8)=18, a(9)=21, a(10)=42, a(11)=63, a(12)=126.
   For n>12 the sequence is defined to be 0. */
#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* Compute the 12 divisors of 126 only once. */
    static seq_t divs[12];
    static int initialized = 0;

    if (!initialized) {
        int pos = 0;
        for (int e2 = 0; e2 <= 1; ++e2) {
            for (int e3 = 0; e3 <= 2; ++e3) {
                for (int e7 = 0; e7 <= 1; ++e7) {
                    seq_t val = (seq_t)1;
                    if (e2) val *= (seq_t)2;
                    if (e3 == 1) val *= (seq_t)3;
                    else if (e3 == 2) val *= (seq_t)9;
                    if (e7) val *= (seq_t)7;
                    divs[pos++] = val;
                }
            }
        }
        /* Simple insertion sort for the 12 elements */
        for (int i = 0; i < 12; ++i) {
            for (int j = i + 1; j < 12; ++j) {
                if (divs[j] < divs[i]) {
                    seq_t tmp = divs[i];
                    divs[i] = divs[j];
                    divs[j] = tmp;
                }
            }
        }
        initialized = 1;
    }

    if (n == 0 || n > (idx_t)12) {
        return (seq_t)0;
    }
    return divs[(size_t)(n - 1)];
}
