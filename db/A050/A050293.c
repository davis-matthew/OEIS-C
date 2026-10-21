/*
 * A050293  Number of 3-fold-free subsets of {1, 2, ..., n}.
 * https://oeis.org/A050293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏ Fib(len_i + 2) over all 3‑multiplication chains of the
 *            form x,3x,9x,… with xᶻ mod 3 ≠ 0; here Fib(0)=0, Fib(1)=1
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 72, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Fibonacci numbers up to F(11) (max required for n ≤ 10000). */
    static const seq_t fib[12] = {0,1,1,2,3,5,8,13,21,34,55,89};

    seq_t result = 1;
    if (n == 0) return result;

    /* Mark numbers already assigned to a chain. */
    bool *visited = (bool *)calloc((size_t)n + 1, sizeof(bool));
    if (!visited) return result;            /* fallback for unlikely allocation failure */

    for (idx_t x = 1; x <= n; ++x) {
        if (x % 3 != 0 && !visited[x]) {
            idx_t len = 1;
            idx_t val = x;
            visited[val] = true;
            while ((val * 3) <= n) {
                val *= 3;
                visited[val] = true;
                ++len;
            }
            result *= fib[(size_t)(len + 2)];
        }
    }

    free(visited);
    return result;
}
