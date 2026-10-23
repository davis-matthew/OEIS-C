/*
 * A165875  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A165875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=15, a(2)=210, ..., a(9)=22136835840 Recurrence for
 *            n>10: a(n) = 13·(a(n-1)+a(n-2)+…+a(n-9)) – 91·a(n-10) Special
 *            case n=10: a(10) = 13·(a(9)+…+a(1)) – 91·a(0) + 1 This
 *            implements the generating‑function recurrence for OEIS A165875.
 *            The function is memoised only by recomputing from sc...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* first 10 terms (0…9) of the sequence */
    static const seq_t init[10] = {
        (seq_t)1ULL,
        (seq_t)15ULL,
        (seq_t)210ULL,
        (seq_t)2940ULL,
        (seq_t)41160ULL,
        (seq_t)576240ULL,
        (seq_t)8067360ULL,
        (seq_t)112943040ULL,
        (seq_t)1581202560ULL,
        (seq_t)22136835840ULL
    };

    if (n < 10)
        return init[(idx_t)n];

    /* buffer[0] = a(i-10), buffer[9] = a(i-1)  for current i */
    seq_t buffer[10];
    for (idx_t i = 0; i < 10; ++i)
        buffer[i] = init[i];

    /* handle n==10 specially */
    if (n == 10) {
        seq_t sum = (seq_t)0;
        for (idx_t j = 1; j < 10; ++j)
            sum += buffer[j];
        seq_t a10 = (sum * (seq_t)13) - (buffer[0] * (seq_t)91) + (seq_t)1;
        return a10;
    }

    /* n > 10: compute a(10) as before, then use recurrence thereafter */
    seq_t sum = (seq_t)0;
    for (idx_t j = 1; j < 10; ++j)
        sum += buffer[j];
    seq_t a10 = (sum * (seq_t)13) - (buffer[0] * (seq_t)91) + (seq_t)1;

    /* slide buffer to contain a(1)…a(10) */
    for (idx_t j = 0; j < 9; ++j)
        buffer[j] = buffer[j + 1];
    buffer[9] = a10;

    /* now compute from i = 11 up to n */
    for (idx_t i = 11; i <= n; ++i) {
        /* compute sum of last 9 values a(i-1)…a(i-9) */
        seq_t s = (seq_t)0;
        for (idx_t j = 1; j < 10; ++j)
            s += buffer[j];

        seq_t val = (s * (seq_t)13) - (buffer[0] * (seq_t)91);

        /* slide buffer left (drop a(i-10)) and insert new value */
        for (idx_t j = 0; j < 9; ++j)
            buffer[j] = buffer[j + 1];
        buffer[9] = val;
    }

    return buffer[9];
}
