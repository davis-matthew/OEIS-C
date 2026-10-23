/*
 * A122180  Number of ways to write n as n = x*y*z with 1 < x < y < z < n.
 * https://oeis.org/A122180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A122180: Number of ways to write n as n = x*y*z with
 *  1 < x < y < z < n,  x, y, z distinct proper factors of n.
 *
 *  The routine enumerates all proper factors (excluding 1 and n),
 *  sorts them and counts unordered triples whose product is exactly n.
 */

#include <stdlib.h>
#include <stdint.h>

/* comparison function for qsort on uint64_t */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb);
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    if (N <= 4)          /* n < 24 quickly gives zero but smaller n certainly zero */
        return 0;

    /* dynamic array of proper divisors (excluding 1 and n) */
    size_t cap = 32;
    uint64_t *divs = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!divs)
        return 0;          /* allocation failure → zero */
    size_t d = 0;

    /* enumerate divisors up to sqrt(N) */
    for (uint64_t i = 2; i * i <= N; ++i) {
        if (N % i == 0) {
            if (d == cap) {
                cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(divs, cap * sizeof(uint64_t));
                if (!tmp) { free(divs); return 0; }
                divs = tmp;
            }
            divs[d++] = i;

            uint64_t j = N / i;
            if (j != i && j != N) {
                if (d == cap) {
                    cap <<= 1;
                    uint64_t *tmp = (uint64_t *)realloc(divs, cap * sizeof(uint64_t));
                    if (!tmp) { free(divs); return 0; }
                    divs = tmp;
                }
                divs[d++] = j;
            }
        }
    }

    /* if fewer than 3 proper divisors, answer is zero */
    if (d < 3) {
        free(divs);
        return 0;
    }

    /* sort divisors to enable x < y < z */
    qsort(divs, d, sizeof(uint64_t), cmp_uint64);

    seq_t count = 0;

    /* count unordered triples */
    for (size_t i = 0; i < d; ++i) {
        uint64_t x = divs[i];
        for (size_t j = i + 1; j < d; ++j) {
            uint64_t y = divs[j];
            unsigned __int128 prod128 = (unsigned __int128)x * y;
            if (prod128 > N)   /* further y will only increase product */
                break;
            uint64_t xy = (uint64_t)prod128;
            if (N % xy != 0)
                continue;
            uint64_t z = N / xy;
            if (z <= y)         /* ensure distinct and ordered */
                continue;
            ++count;
        }
    }

    free(divs);
    return count;
}
