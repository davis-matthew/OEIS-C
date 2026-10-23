/*
 * A135615  a(n) = number of positive divisors of (n+1) that are each the
 *          average of a positive divisor of n and a positive divisor of
 *          (n+2).
 * https://oeis.org/A135615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of (n+1) such that there exist * a
 *            divisor x of n and a divisor y of (n+2) with d = (x + y)/2
 * Range:     u8 n <= 2500, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static void get_divisors(uint64_t m, uint64_t **out, size_t *len)
{
    size_t cap = 16;
    uint64_t *arr = (uint64_t *)malloc(cap * sizeof(uint64_t));
    *len = 0;

    for (uint64_t i = 1; i * i <= m; ++i) {
        if (m % i == 0) {
            if (*len == cap) {
                cap <<= 1;
                arr = (uint64_t *)realloc(arr, cap * sizeof(uint64_t));
            }
            arr[(*len)++] = i;
            if (i != m / i) {
                if (*len == cap) {
                    cap <<= 1;
                    arr = (uint64_t *)realloc(arr, cap * sizeof(uint64_t));
                }
                arr[(*len)++] = m / i;
            }
        }
    }
    *out = arr;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t n1 = N + 1;
    uint64_t n2 = N + 2;

    uint64_t *dv_n  = NULL, *dv_n1 = NULL, *dv_n2 = NULL;
    size_t   len_n  = 0,   len_n1 = 0,   len_n2 = 0;

    get_divisors(N,  &dv_n,  &len_n);
    get_divisors(n1, &dv_n1, &len_n1);
    get_divisors(n2, &dv_n2, &len_n2);

    seq_t count = 0;

    /* For each divisor d of n1, check if there exists x in dv_n and y in dv_n2
     * with x + y = 2*d.  Use a simple set test by scanning dv_n2. */
    for (size_t i = 0; i < len_n1; ++i) {
        uint64_t d = dv_n1[i];
        uint64_t target = d << 1;          /* 2*d, fits in uint64_t for given n */
        bool found = false;

        for (size_t j = 0; j < len_n && !found; ++j) {
            uint64_t x = dv_n[j];
            if (target <= x) continue;     /* y would be non‑positive */
            uint64_t y = target - x;
            for (size_t k = 0; k < len_n2; ++k) {
                if (dv_n2[k] == y) {
                    found = true;
                    break;
                }
            }
        }
        if (found) ++count;
    }

    free(dv_n);
    free(dv_n1);
    free(dv_n2);

    return (seq_t)count;
}
