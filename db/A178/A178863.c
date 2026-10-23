/*
 * A178863  Divisors of 25200.
 * https://oeis.org/A178863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 25200 in ascending order */

#include <stdbool.h>
#include <stddef.h>

static seq_t divs[90];
static bool divs_inited = false;

static void init_divisors(void)
{
    if (divs_inited) return;

    unsigned int idx = 0;
    /* build all products 2^e2 * 3^e3 * 5^e5 * 7^e7  (e2=0..4, e3=0..2, e5=0..2, e7=0..1) */
    for (unsigned int e2 = 0; e2 <= 4; ++e2) {
        seq_t p2 = 1;
        for (unsigned int i = 0; i < e2; ++i) p2 *= 2;
        for (unsigned int e3 = 0; e3 <= 2; ++e3) {
            seq_t p3 = p2;
            for (unsigned int i = 0; i < e3; ++i) p3 *= 3;
            for (unsigned int e5 = 0; e5 <= 2; ++e5) {
                seq_t p5 = p3;
                for (unsigned int i = 0; i < e5; ++i) p5 *= 5;
                for (unsigned int e7 = 0; e7 <= 1; ++e7) {
                    seq_t val = p5;
                    if (e7) val *= 7;
                    divs[idx++] = val;
                }
            }
        }
    }

    /* insertion sort – array size is only 90 */
    for (unsigned int i = 1; i < 90; ++i) {
        seq_t key = divs[i];
        unsigned int j = i;
        while (j > 0 && divs[j - 1] > key) {
            divs[j] = divs[j - 1];
            --j;
        }
        divs[j] = key;
    }

    divs_inited = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for zero index */
    init_divisors();
    if (n <= 90) return divs[n - 1];
    return 0;                       /* beyond the finite sequence */
}
