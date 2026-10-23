/*
 * A106672  Marks on lexicographically fourth 7-mark optimal Golomb ruler.
 * https://oeis.org/A106672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Enumerates 7‑mark optimal Golomb rulers (minimal length 25)
   and returns the marks of the fourth lexicographically smallest one
   (a(0)=0, a(1)=2, a(2)=3, a(3)=10, a(4)=16, a(5)=21, a(6)=25). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t fourth[7];
static bool initialized = false;

seq_t a(idx_t n)
{
    if (!initialized) {
        size_t counter = 0;
        for (int a = 1; a <= 20; ++a) {
            for (int b = a + 1; b <= 21; ++b) {
                for (int c = b + 1; c <= 22; ++c) {
                    for (int d = c + 1; d <= 23; ++d) {
                        for (int e = d + 1; e <= 24; ++e) {
                            int marks[7] = { 0, a, b, c, d, e, 25 };
                            uint32_t mask = 0;
                            bool ok = true;
                            for (int i = 0; i < 7 && ok; ++i) {
                                for (int j = i + 1; j < 7; ++j) {
                                    int diff = marks[j] - marks[i];
                                    uint32_t bit = 1u << diff;
                                    if (mask & bit) { ok = false; break; }
                                    mask |= bit;
                                }
                            }
                            if (ok) {
                                ++counter;
                                if (counter == 4) {
                                    for (int i = 0; i < 7; ++i)
                                        fourth[i] = (seq_t)marks[i];
                                }
                            }
                        }
                    }
                }
            }
        }
        initialized = true;
    }
    return (n < 7) ? fourth[(size_t)n] : (seq_t)0;
}
