/*
 * A002705  Sets with a congruence property.
 * https://oeis.org/A002705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   with N = 2n+1 and m = N/e, a(n) = (1/(5N)) * Sum_{e|N} S(e) - 1,
 *            where 5 !| e, 5 !| m: S(e) = phi(e)*(4^m + 1) 5 !| e, 5 | m:
 *            S(e) = phi(e)*(4^m - 4) 5 | e, 5 !| m: S(e) = 5*phi(e)/4 5 | e,
 *            5 | m: S(e) = -5*phi(e) (character-sum count of nonempty subsets
 *            of {r mod 5N : r mod 5 in {1,2}} with sum...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-01)
 * Helpers:   totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient_u64.h"

static int a2705_bits(void)
{
    static int b = -1;
    if (b < 0) {
        seq_t x = 1;
        int c = 0;
        while (x != 0 && c < 300) {
            x = x << 1;
            c++;
        }
        b = (x == 0) ? c : 0;
    }
    return b;
}

static seq_t a2705_mul(seq_t x, seq_t y)
{
    seq_t r = (x + 0u) * y;
    return r;
}

static seq_t a2705_pow4(idx_t m, int w)
{
    seq_t one = 1;
    if (w && 2 * m >= (idx_t)w)
        return 0;
    return one << (2 * m);
}

seq_t a(idx_t n)
{
    idx_t N = 2 * n + 1, e;
    int w = a2705_bits();
    seq_t pos = 0, neg = 0;

    for (e = 1; e <= N; e++) {
        idx_t m;
        uint64_t ph;
        if (N % e != 0)
            continue;
        m = N / e;
        ph = totient_u64(e);
        if (e % 5 != 0) {
            seq_t P = (seq_t)ph;
            seq_t pw = a2705_pow4(m, w);
            pos += a2705_mul(pw, P);
            if (m % 5 != 0)
                pos += P;
            else
                neg += (seq_t)(4 * ph);
        } else {
            if (m % 5 == 0)
                neg += (seq_t)(5 * ph);
            else
                pos += (seq_t)(5 * (ph / 4));
        }
    }

    if (w == 0) {
        seq_t q = (seq_t)(5 * N);
        return (pos - neg) / q - 1;
    } else {
        seq_t total = pos - neg;
        seq_t q = (seq_t)(5 * N);
        seq_t inv = q;
        int i;
        for (i = 0; i < 8; i++) {
            seq_t t = a2705_mul(q, inv);
            t = 2 - t;
            inv = a2705_mul(inv, t);
        }
        total = a2705_mul(total, inv);
        total = total - 1;
        return total;
    }
}
