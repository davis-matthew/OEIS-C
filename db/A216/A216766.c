/*
 * A216766  Numerators of partial sums of 1/A216765(n).
 * https://oeis.org/A216766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 18, u128 n <= 32,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A216766: numerator of partial sums of 1/(perfect powers + 1) */
/* Computed by generating perfect powers up to a bound, sorting, deduplicating,
   and summing reciprocals. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    if (va < vb) return -1;
    return va > vb ? 1 : 0;
}

static void generate_powers(uint64_t limit, uint64_t **out, size_t *sz)
{
    size_t cap = 1024;
    uint64_t *arr = (uint64_t *)malloc(cap * sizeof(uint64_t));
    size_t count = 0;

    for (uint64_t base = 2; base * base <= limit; ++base) {
        uint64_t power = base * base;
        while (power <= limit) {
            if (count >= cap) {
                cap *= 2;
                arr = (uint64_t *)realloc(arr, cap * sizeof(uint64_t));
            }
            arr[count++] = power;
            if (power > limit / base) break;
            power *= base;
        }
    }

    qsort(arr, count, sizeof(uint64_t), cmp_uint64);

    size_t uniq = 0;
    for (size_t i = 0; i < count; ++i) {
        if (uniq == 0 || arr[i] != arr[uniq - 1]) {
            arr[uniq++] = arr[i];
        }
    }

    *out = arr;
    *sz  = uniq;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    uint64_t limit = n > 1 ? (uint64_t)n * (uint64_t)n : (uint64_t)10;
    uint64_t *powers = NULL;
    size_t cnt = 0;

    while (1) {
        generate_powers(limit, &powers, &cnt);
        if (cnt >= (size_t)n) break;
        free(powers);
        limit <<= 1;            /* double the bound and try again */
    }

    seq_t num = 0;
    seq_t den = 1;
    for (size_t i = 0; i < n; ++i) {
        seq_t d = (seq_t)(powers[i] + 1);   /* denominator = power + 1 */
        seq_t new_num = num * d + den;
        seq_t new_den = den * d;
        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    free(powers);
    return num;
}
