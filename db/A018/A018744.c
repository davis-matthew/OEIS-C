/*
 * A018744  Divisors of 960.
 * https://oeis.org/A018744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n‑th smallest divisor of 960
 * Range:     u8 n <= 19, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static int cmp_seq_t(const void *p1, const void *p2)
{
    seq_t x = *(const seq_t *)p1;
    seq_t y = *(const seq_t *)p2;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

static void init_divisors(seq_t arr[28])
{
    unsigned int idx = 0;
    for (unsigned int a = 0; a <= 6; ++a) {
        seq_t va = (seq_t)1 << a;          // 2^a
        for (unsigned int b = 0; b <= 1; ++b) {
            seq_t vb = b ? (seq_t)3 : (seq_t)1;   // 3^b
            for (unsigned int c = 0; c <= 1; ++c) {
                seq_t vc = c ? (seq_t)5 : (seq_t)1; // 5^c
                arr[idx++] = va * vb * vc;
            }
        }
    }
    qsort(arr, 28, sizeof(seq_t), cmp_seq_t);
}

seq_t a(idx_t n)
{
    static seq_t divisors[28];
    static bool initialized = false;
    if (!initialized) {
        init_divisors(divisors);
        initialized = true;
    }
    if (n >= 1 && n <= 28) {
        return divisors[(size_t)(n - 1)];
    }
    return (seq_t)0;
}
