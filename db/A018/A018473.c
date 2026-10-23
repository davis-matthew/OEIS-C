/*
 * A018473  Divisors of 472.
 * https://oeis.org/A018473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 472: a(n) is the n-th smallest divisor of 472
   (OEIS sequence A018473).  The divisors are 1,2,4,8,59,118,236,472. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static storage for the computed divisors */
    static seq_t divs[32];
    static size_t div_count = 0;

    if (div_count == 0) {
        /* The constant 472 is small; factorise it directly. */
        const uint64_t base = 472ULL;
        size_t idx = 0;

        /* Collect all divisors (both small and large). */
        for (uint64_t i = 1; i * i <= base; ++i) {
            if (base % i == 0) {
                divs[idx++] = (seq_t)i;          /* small divisor */
                if (i != base / i)                /* large divisor */
                    divs[idx++] = (seq_t)(base / i);
            }
        }
        /* Now sort the divisor list in ascending order. */
        for (size_t i = 1; i < idx; ++i) {
            seq_t key = divs[i];
            size_t j = i;
            while (j > 0 && divs[j - 1] > key) {
                divs[j] = divs[j - 1];
                --j;
            }
            divs[j] = key;
        }
        div_count = idx;
    }

    /* Bounds check: a(1) is the first term. */
    if (n == 0 || n > div_count) {
        return (seq_t)0;
    }

    return divs[n - 1];
}
