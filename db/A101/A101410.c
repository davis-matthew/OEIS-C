/*
 * A101410  Divisors of 100K, reversed.
 * https://oeis.org/A101410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, full, nonn, base, fini
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 34, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = reverse of the decimal representation of the nth divisor of 100000 (2^5·5^5) */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/cmp_seq_t.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* generate the 36 divisors of 100000 = 2^5 * 5^5 */
    static seq_t divisors[36];
    static int initialized = 0;
    if (!initialized) {
        seq_t pow5[6];
        pow5[0] = 1;
        for (int k = 1; k <= 5; ++k)
            pow5[k] = pow5[k - 1] * (seq_t)5;

        int idx = 0;
        for (int i = 0; i <= 5; ++i) {
            seq_t pow2 = ((seq_t)1 << i);
            for (int j = 0; j <= 5; ++j)
                divisors[idx++] = pow2 * pow5[j];
        }
        qsort(divisors, 36, sizeof(seq_t), cmp_seq_t);
        initialized = 1;
    }

    if (n > 36) return 0; /* sequence defined only for first 36 terms */

    seq_t d = divisors[n - 1];
    seq_t rev = 0;
    while (d > 0) {
        rev = rev * 10 + d % 10;
        d /= 10;
    }
    return rev;
}
