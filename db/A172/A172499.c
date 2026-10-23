/*
 * A172499  a(n) = numerator of fraction whose decimal representation is
 *          (n).(1)(2)(3)...(n-1)(n)... with repeating part
 *          (1)(2)(3)...(n-1)(n).
 * https://oeis.org/A172499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, frac
 * Formula:   a(n) = ((10^L – 1)*n + U)/gcd(((10^L – 1)*n + U),(10^L – 1)),
 *            where L = Σ_{k=1..n} digit_count(k) and U = concat(1,2,…,n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   digit_count, gcd, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/gcd.h"
#include "../utils/digit_count.h"

seq_t a(idx_t n)
{
    static seq_t pow10_digit[20];
    static bool init = false;
    if (!init) {
        pow10_digit[0] = (seq_t)1;
        for (idx_t d = 1; d < 20; ++d)
            pow10_digit[d] = pow10(d);
        init = true;
    }

    seq_t U = (seq_t)0;
    idx_t L = 0;
    for (idx_t k = 1; k <= n; ++k) {
        uint64_t d = digit_count(k);
        L += d;
        seq_t mult = pow10_digit[d];
        seq_t kseq = (seq_t)k;
        U = U * mult + kseq;
    }

    seq_t ten_pow_L = pow10(L);
    seq_t D = ten_pow_L - (seq_t)1;
    seq_t n_seq = (seq_t)n;
    seq_t raw_num = n_seq * D + U;
    seq_t g = gcd(raw_num, D);
    seq_t ans = raw_num / g;
    return ans;
}
