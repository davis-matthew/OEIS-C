/*
 * A135134  Emirps that use all of the prime digits 2,3,5,7 exactly once.
 * https://oeis.org/A135134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   Emirps that use all prime digits 2,3,5,7 exactly once (generated
 *            by checking all 24 permutations)
 * Range:     u8 none, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime_big, seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_cmp.h"

static seq_t seq_arr[4];
static idx_t seq_count = 0;
static bool seq_initialized = false;

static int cmp_seq(const void *a, const void *b)
{
    return seq_cmp(a, b);
}

seq_t a(idx_t n)
{
    if (!seq_initialized) {
        /* generate all permutations of digits 2,3,5,7 */
        int digits[4] = {2, 3, 5, 7};
        idx_t m = 0;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) if (j != i) {
                for (int k = 0; k < 4; ++k) if (k != i && k != j) {
                    for (int l = 0; l < 4; ++l) if (l != i && l != j && l != k) {
                        uint64_t num = 0;
                        int d[4] = {digits[i], digits[j], digits[k], digits[l]};
                        for (int t = 0; t < 4; ++t)
                            num = num * 10 + d[t];
                        uint64_t rev = 0;
                        for (int t = 3; t >= 0; --t)
                            rev = rev * 10 + d[t];
                        if (is_prime_big((seq_t)num) && is_prime_big((seq_t)rev)) {
                            if (m < 4)
                                seq_arr[m++] = (seq_t)num;
                        }
                    }
                }
            }
        }
        qsort(seq_arr, m, sizeof(seq_t), cmp_seq);
        seq_count = m;
        seq_initialized = true;
    }
    if (n <= seq_count)
        return seq_arr[n - 1];
    else
        return (seq_t)0;
}
