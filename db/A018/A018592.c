/*
 * A018592  Divisors of 686.
 * https://oeis.org/A018592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th smallest divisor of 686 (in increasing order)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n > 8 the sequence has no defined terms; this implementation returns 0. */

#include <stdbool.h>
#include <stddef.h>

/* The sequence has at most 8 divisors. */
#define MAX_D 8

seq_t a(idx_t n)
{
    static seq_t divs[MAX_D];
    static size_t divcnt = 0;
    static bool initialized = false;

    if (!initialized) {
        const seq_t N = (seq_t)686;          /* the number whose divisors we need */
        seq_t tmp[MAX_D];
        size_t cnt = 0;

        /* Find all divisors by trial division up to sqrt(N) */
        for (idx_t i = 1; (seq_t)i * (seq_t)i <= N; ++i) {
            if (N % (seq_t)i == 0) {
                tmp[cnt++] = (seq_t)i;
                seq_t j = N / (seq_t)i;
                if (j != (seq_t)i) {
                    tmp[cnt++] = j;
                }
            }
        }

        /* Simple insertion sort (cnt <= MAX_D) */
        for (size_t i = 1; i < cnt; ++i) {
            seq_t key = tmp[i];
            int j = (int)i - 1;
            while (j >= 0 && tmp[(size_t)j] > key) {
                tmp[(size_t)(j + 1)] = tmp[(size_t)j];
                --j;
            }
            tmp[(size_t)(j + 1)] = key;
        }

        /* Copy sorted divisors into the static array */
        for (size_t i = 0; i < cnt; ++i) {
            divs[i] = tmp[i];
        }
        divcnt = cnt;
        initialized = true;
    }

    /* 1‑based indexing */
    if (n >= 1 && n <= divcnt) {
        return divs[n - 1];
    } else {
        return 0;   /* Undefined term */
    }
}
