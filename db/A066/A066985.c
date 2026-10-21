/*
 * A066985  Reflective numbers: k such that the decimal encoding of the prime
 *          factorization of k (A067599) is palindromic.
 * https://oeis.org/A066985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the n-th integer whose prime factorization encoding
 *            (prime followed by exponent, concatenated as decimal digits) is
 *            a palindrome. The function searches numbers sequentially,
 *            checking the factorization and palindrome property
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    uint64_t target = (uint64_t)n;
    uint64_t count = 0;

    for (uint64_t k = 2; ; ++k) {
        size_t factor_cnt = 0;
        uint64_t primes[64];
        uint64_t exps  [64];
        factor_u64(k, primes, exps, &factor_cnt);

        char enc[512];
        int  len = 0;
        char tmp[32];

        for (size_t i = 0; i < factor_cnt; ++i) {
            int l = snprintf(tmp, sizeof tmp, "%llu", primes[i]);
            memcpy(enc + len, tmp, l);
            len += l;

            l = snprintf(tmp, sizeof tmp, "%llu", exps[i]);
            memcpy(enc + len, tmp, l);
            len += l;
        }

        bool pal = true;
        for (int i = 0; i < len / 2; ++i) {
            if (enc[i] != enc[len - 1 - i]) { pal = false; break; }
        }
        if (pal) {
            ++count;
            if (count == target) return (seq_t)k;
        }
    }
}
