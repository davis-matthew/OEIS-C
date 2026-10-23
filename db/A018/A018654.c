/*
 * A018654  Divisors of 801.
 * https://oeis.org/A018654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A018654: divisors of 801 in ascending order (by search) */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t divs[16];
static size_t divcnt = 0;
static bool initialized = false;

seq_t a(idx_t n)
{
    if (!initialized) {
        const uint64_t base = 801u;
        size_t cnt = 0;

        /* Find all divisors of 801. */
        for (uint64_t d = 1; d * d <= base; ++d) {
            if (base % d == 0) {
                divs[cnt++] = (seq_t)d;
                uint64_t e = base / d;
                if (e != d) {
                    divs[cnt++] = (seq_t)e;
                }
            }
        }

        /* Sort the found divisors in ascending order. */
        for (size_t i = 0; i + 1 < cnt; ++i) {
            for (size_t j = i + 1; j < cnt; ++j) {
                if (divs[j] < divs[i]) {
                    seq_t tmp = divs[i];
                    divs[i]    = divs[j];
                    divs[j]    = tmp;
                }
            }
        }

        divcnt = cnt;
        initialized = true;
    }

    if (n == 0) return (seq_t)0;          /* offset is 1; just in case */
    if ((size_t)(n - 1) < divcnt) {
        return divs[n - 1];
    }
    return (seq_t)0;                       /* values beyond the finite list */
}
