/*
 * A003226  Automorphic numbers: m^2 ends with m.
 * https://oeis.org/A003226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice, easy
 * Formula:   a(1)=0, a(2)=1; for each k >= 1 the k-digit terms are e5_k =
 *            5^(2^k) mod 10^k and e6_k = 10^k + 1 - e5_k, each kept only if
 *            its leading digit is nonzero, then the two are sorted. The
 *            digits of e5 come from Hensel lifting: digit k = (digit k of
 *            e5_k^2) mod 10
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 19, u64 n <= 36, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static unsigned char *A3226_D = NULL;
static long long A3226_len = 0;
static long long A3226_cap = 0;
static long long A3226_carry = 0;

static void A3226_ensure(long long k)
{
    if (A3226_len == 0) {
        A3226_cap = 64;
        A3226_D = (unsigned char *)malloc((size_t)A3226_cap);
        A3226_D[0] = 5;
        A3226_len = 1;
        A3226_carry = 2; /* 25 / 10 */
    }
    while (A3226_len < k) {
        long long p = A3226_len;
        long long s, i;
        int d;
        if (p >= A3226_cap) {
            A3226_cap *= 2;
            A3226_D = (unsigned char *)realloc(A3226_D, (size_t)A3226_cap);
        }
        s = A3226_carry;
        for (i = 1; i < p; i++)
            s += (long long)A3226_D[i] * A3226_D[p - i];
        d = (int)(s % 10);
        A3226_D[p] = (unsigned char)d;
        A3226_carry = (s + 10LL * d) / 10;
        A3226_len++;
    }
}

static seq_t A3226_build(long long k, bool six)
{
    seq_t v = 0;
    long long i;
    for (i = k - 1; i >= 0; i--) {
        int dig;
        if (six)
            dig = (i == 0) ? 6 : 9 - (int)A3226_D[i];
        else
            dig = (int)A3226_D[i];
        v = v * 10 + (seq_t)dig;
    }
    return v;
}

seq_t a(idx_t n)
{
    idx_t idx = 2;
    long long k;
    if (n == 1) return 0;
    if (n == 2) return 1;
    for (k = 1;; k++) {
        bool first_six, has5, has6;
        int top;
        A3226_ensure(k);
        if (k == 1) {
            has5 = true;
            has6 = true;
            first_six = false;
        } else {
            top = (int)A3226_D[k - 1];
            has5 = (top != 0);
            has6 = (top != 9);
            first_six = (9 - top < top);
        }
        if (first_six) {
            if (has6) { idx++; if (idx == n) return A3226_build(k, true); }
            if (has5) { idx++; if (idx == n) return A3226_build(k, false); }
        } else {
            if (has5) { idx++; if (idx == n) return A3226_build(k, false); }
            if (has6) { idx++; if (idx == n) return A3226_build(k, true); }
        }
    }
}
