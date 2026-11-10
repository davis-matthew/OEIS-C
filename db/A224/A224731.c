/*
 * A224731  b(n+1) - b(n) + n where b(n) = A095114(n).
 * https://oeis.org/A224731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(n+1) - b(n) + n, where b = A095114: b(1)=1, b(n) =
 *            b(n-1) + #{k<n : b(k) <= n-1}
 * Range:     u8 n <= 221, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Validated: standard, pass, 76 terms through n = 7419 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    idx_t size = n + 2;
    seq_t *arr = mem_new(seq_t, size);

    arr[1] = (seq_t)1;
    idx_t j = 0;
    for (idx_t i = 2; i <= n + 1; ++i) {
        while (j + 1 <= i - 1 && arr[j + 1] <= (seq_t)(i - 1)) {
            ++j;
        }
        arr[i] = arr[i - 1] + (seq_t)j;
    }

    seq_t result = arr[n + 1] - arr[n] + (seq_t)n;
    mem_free(arr);
    return result;
}
