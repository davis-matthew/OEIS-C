/*
 * A018755  Divisors of 978.
 * https://oeis.org/A018755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: divisors of 978 sorted ascending */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* 0‑based divisor list is sorted. If n exceeds the number of
       divisors, 0 is returned. */
    if (n == (idx_t)0) return (seq_t)0;        /* undefined for n<1 */

    const uint64_t N = 978;
    seq_t divisors[16];
    size_t count = 0;

    /* Generate the divisors by checking all numbers up to sqrt(N) */
    for (uint64_t i = 1; i * i <= N; ++i) {
        if (N % i == 0) {
            divisors[count++] = (seq_t)i;
            if (i != N / i) {
                divisors[count++] = (seq_t)(N / i);
            }
        }
    }

    /* Sort the divisor list (bubble sort, O(count²), but count ≤ 8) */
    for (size_t i = 0; i < count; ++i) {
        for (size_t j = i + 1; j < count; ++j) {
            if (divisors[i] > divisors[j]) {
                seq_t tmp = divisors[i];
                divisors[i] = divisors[j];
                divisors[j] = tmp;
            }
        }
    }

    if (n <= (idx_t)count) return divisors[n - 1];
    return (seq_t)0;   /* no such divisor */
}
