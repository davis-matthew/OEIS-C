/*
 * A078811  a(n) = A078217(n)/n.
 * https://oeis.org/A078811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A078217(n)/n, where A078217(n) is the smallest multiple
 *            of n beginning with the concatenation of the integers <= n
 *            coprime to n. Computed by a search for the least d with (-C*10^d
 *            mod n) < 10^d, then long division by n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-07)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd_u64.h"

seq_t a(idx_t n)
{
    uint64_t n64 = (uint64_t)n;
    size_t total = 0;
    for (uint64_t k = 1; k <= n64; ++k) {
        if (gcd_u64(k, n64) == 1) {
            uint64_t x = k;
            do { total++; x /= 10; } while (x);
        }
    }
    /* digits of C */
    unsigned char *dig = (unsigned char *)malloc(total + 64 + 1);
    size_t len = 0;
    unsigned __int128 Cmod = 0;
    for (uint64_t k = 1; k <= n64; ++k) {
        if (gcd_u64(k, n64) == 1) {
            unsigned char tmp[24];
            int t = 0;
            uint64_t x = k;
            do { tmp[t++] = (unsigned char)(x % 10); x /= 10; } while (x);
            while (t > 0) {
                unsigned char dg = tmp[--t];
                dig[len++] = dg;
                Cmod = (Cmod * 10 + dg) % n64;
            }
        }
    }
    /* find least d */
    uint64_t pm = 1 % n64;
    uint64_t p10 = 1;
    uint64_t need = 0;
    int d = 0;
    for (d = 0; ; ++d) {
        uint64_t prod = (uint64_t)((Cmod * pm) % n64);
        need = (n64 - prod) % n64;
        if (d >= 20 || need < p10) break;
        pm = (uint64_t)(((unsigned __int128)pm * 10) % n64);
        p10 *= 10;
    }
    /* append d digits of need (zero padded) */
    unsigned char *tail = (unsigned char *)malloc((size_t)d + 1);
    {
        uint64_t x = need;
        for (int i = d - 1; i >= 0; --i) {
            tail[i] = (unsigned char)(x % 10);
            x /= 10;
        }
    }
    unsigned char *all = (unsigned char *)malloc(len + (size_t)d + 1);
    for (size_t i = 0; i < len; ++i) all[i] = dig[i];
    for (int i = 0; i < d; ++i) all[len + (size_t)i] = tail[i];
    size_t L = len + (size_t)d;

    /* long division by n */
    seq_t q = 0;
    unsigned __int128 r = 0;
    for (size_t i = 0; i < L; ++i) {
        r = r * 10 + all[i];
        unsigned __int128 qd = r / n64;
        r = r % n64;
        q = q * 10 + (seq_t)(uint64_t)qd;
    }
    free(dig);
    free(tail);
    free(all);
    return q;
}
