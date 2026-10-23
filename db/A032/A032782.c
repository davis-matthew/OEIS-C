/*
 * A032782  Numbers k such that k(k+1)(k+2)...(k+9) /
 *          (k+(k+1)+(k+2)+...+(k+9)) is an integer.
 * https://oeis.org/A032782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = (d-9)/2 where d is the nth divisor >=9 of 178605
 * Range:     u8 n <= 7, u16 n <= 36, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of all k such that k(k+1)…(k+9) / (k+…+k+9) is an integer. */
/* Property: 178605 = 5·3⁶·7², so k = (d‑9)/2 for divisors d≥9 of 178605. */

#include <stdlib.h>
#include <stdbool.h>

static int cmp_seq_t(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

static void init_a032782_table(seq_t *table, size_t *len)
{
    /* Prime factorisation: 178605 = 5 · 3⁶ · 7² */
    const int e3max = 6;
    const int e5max = 1;
    const int e7max = 2;

    size_t idx = 0;

    for (int e3 = 0; e3 <= e3max; ++e3) {
        uint64_t pow3 = 1ULL;
        for (int i = 0; i < e3; ++i) pow3 *= 3ULL;
        for (int e5 = 0; e5 <= e5max; ++e5) {
            uint64_t pow5 = 1ULL;
            if (e5) pow5 = 5ULL;
            for (int e7 = 0; e7 <= e7max; ++e7) {
                uint64_t pow7 = 1ULL;
                if (e7 == 1) pow7 = 7ULL;
                else if (e7 == 2) pow7 = 49ULL;
                uint64_t d = pow3 * pow5 * pow7;
                if (d >= 9ULL) {
                    table[idx++] = (seq_t)((d - 9ULL) / 2ULL);
                }
            }
        }
    }

    *len = idx;
    /* Sort the list of k-values increasingly */
    qsort(table, idx, sizeof(seq_t), cmp_seq_t);
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t table[42];      /* 7·2·3 = 42 divisors of 178605 */
    static size_t count = 0;

    if (!initialized) {
        init_a032782_table(table, &count);
        initialized = true;
    }

    if (n == 0 || n > count) return (seq_t)0;
    return table[n - 1];
}
