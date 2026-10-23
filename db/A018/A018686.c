/*
 * A018686  Divisors of 856.
 * https://oeis.org/A018686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 856 in ascending order
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static bool init_done = false;
static seq_t divs[8];

static void init_divisors(void)
{
    if (init_done) return;
    idx_t idx = 0;
    for (int e107 = 0; e107 <= 1; e107++) {
        seq_t pow107 = (e107 ? (seq_t)107 : (seq_t)1);
        for (int e2 = 0; e2 <= 3; e2++) {
            seq_t pow2 = (seq_t)1;
            for (int i = 0; i < e2; i++) {
                pow2 *= (seq_t)2;
            }
            divs[idx++] = pow107 * pow2;
        }
    }
    init_done = true;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    init_divisors();
    if (n <= 8) return divs[(size_t)(n-1)];
    return (seq_t)0;
}
