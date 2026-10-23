/*
 * A032778  Numbers k such that k*(k+1)*(k+2)*...*(k+7) /
 *          (k+(k+1)+(k+2)+...+(k+7)) is an integer.
 * https://oeis.org/A032778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A032778: numbers k such that k*(k+1)*...*(k+7) / (k+...+(k+7)) is an integer.
   Equivalently, k = (d-7)/2 where d ≥ 7 is a divisor of 11025. */
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache of the sequence values. */
    static bool ready = false;
    static seq_t table[27];
    static size_t length = 0;

    if (!ready) {
        /* Precompute all divisors of 11025 = 3^2 * 5^2 * 7^2. */
        const uint64_t pow3[3] = {1, 3, 9};
        const uint64_t pow5[3] = {1, 5, 25};
        const uint64_t pow7[3] = {1, 7, 49};

        size_t cnt = 0;
        for (int a = 0; a <= 2; ++a) {
            for (int b = 0; b <= 2; ++b) {
                for (int c = 0; c <= 2; ++c) {
                    uint64_t d = pow3[a] * pow5[b] * pow7[c];
                    if (d >= 7) {
                        /* k = (d-7)/2 fits in 64 bits and in seq_t. */
                        table[cnt++] = (seq_t)((d - 7ULL) / 2);
                    }
                }
            }
        }
        /* Sort the generated values in ascending order. */
        for (size_t i = 1; i < cnt; ++i) {
            seq_t key = table[i];
            size_t j = i;
            while (j > 0 && table[j - 1] > key) {
                table[j] = table[j - 1];
                --j;
            }
            table[j] = key;
        }
        length = cnt;
        ready = true;
    }

    if (n == 0 || n > (idx_t)length)
        return (seq_t)0;        /* Undefined beyond the finite sequence. */
    return table[(size_t)(n - 1)];
}
