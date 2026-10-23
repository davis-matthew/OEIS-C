/*
 * A123738  Partial sums of (-1)^floor(n*Pi).
 * https://oeis.org/A123738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = sum_{k=1..n} (-1)^floor(k * PI)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static const long double PI = 3.14159265358979323846264338327950288L;

seq_t a(idx_t n)
{
    static seq_t *memo = NULL;     /* memoized partial sums */
    static idx_t memo_len = 0;     /* number of terms stored in memo */
    static idx_t memo_cap = 0;     /* allocated capacity of memo */
    static seq_t sum = 0;          /* current running sum a(memo_len) */

    if (n == 0)
        return 0;                   /* undefined for n<1, but return 0 for safety */

    /* Allocate or grow memo if needed */
    if (n > memo_len) {
        size_t needed = (size_t)n;
        if (needed > memo_cap) {
            size_t new_cap = memo_cap ? memo_cap * 2 : 1;
            while (new_cap < needed) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
            memo = tmp;
            memo_cap = new_cap;
        }

        /* Compute the missing terms */
        for (idx_t i = memo_len + 1; i <= n; ++i) {
            /* floor(i * PI) using long double for precision */
            long double prod = (long double)i * PI;
            uint64_t floor_val = (uint64_t)prod;
            int sgn = (floor_val & 1ULL) ? -1 : 1;  /* (-1)^floor(i*PI) */
            sum += (seq_t)sgn;
            memo[i - 1] = sum;                    /* store a(i) */
        }
        memo_len = n;
    }

    return memo[n - 1];
}
