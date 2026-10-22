/*
 * A000905  Hamilton numbers.
 * https://oeis.org/A000905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 2 + Sum_{m=1..n-1} (-1)^(m+1) * binomial(a(n-m), m+1),
 *            a(1) = 2
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   hb_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/hb_binom.h"

#define HAM_MAX 64

static seq_t ham_memo[HAM_MAX + 1];
static idx_t ham_cnt = 0;

seq_t a(idx_t n)
{
    if (n < 1 || n > HAM_MAX)
        return (seq_t)0;
    if (ham_cnt == 0) {
        ham_memo[1] = (seq_t)2;
        ham_cnt = 1;
    }
    while (ham_cnt < n) {
        idx_t k = ham_cnt + 1;
        seq_t pos = (seq_t)2;
        seq_t neg = (seq_t)0;
        for (idx_t m = 1; m <= k - 1; m++) {
            seq_t t = hb_binom(ham_memo[k - m], m + 1);
            if (m % 2 == 1)
                pos += t;
            else
                neg += t;
        }
        ham_memo[k] = pos - neg;
        ham_cnt = k;
    }
    return ham_memo[n];
}
