/*
 * A133220  a(n) is the smallest positive number k such that k and n*k are
 *          anagrams.
 * https://oeis.org/A133220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smallest positive k such that k and n*k have identical
   multiset of decimal digits.  The algorithm searches k from 1 upwards,
   first comparing digit sums (cheap) and then full digit counts when
   necessary. */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Count decimal digits of a seq_t number into a 10‑element array. */
static inline void digit_counts(seq_t x, unsigned char cnt[10])
{
    memset(cnt, 0, 10);
    if (x == 0) {
        cnt[0] = 1;
        return;
    }
    while (x != 0) {
        int d = (int)(x % 10);
        cnt[d]++;
        x /= 10;
    }
}

/* Sum of decimal digits of a seq_t number. */
static inline seq_t digit_sum(seq_t x)
{
    seq_t sum = 0;
    while (x != 0) {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t k = 1;
    unsigned char cnt_k[10], cnt_p[10];
    for (;;) {
        seq_t prod = k * n_seq;
        if (digit_sum(k) == digit_sum(prod)) {
            digit_counts(k, cnt_k);
            digit_counts(prod, cnt_p);
            bool equal = true;
            for (int i = 0; i < 10; ++i) {
                if (cnt_k[i] != cnt_p[i]) {
                    equal = false;
                    break;
                }
            }
            if (equal) return k;
        }
        ++k;
    }
}
