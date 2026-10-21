/*
 * A075253  Trajectory of 77 under the Reverse and Add! operation carried out
 *          in base 2.
 * https://oeis.org/A075253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   Piecewise for n ≥ 3 n ≡ 3 (mod 4): a(n) = 48·2^(2k) – 21·2^k
 *            where k = (n+5)/4 n ≡ 0 (mod 4): a(0)=77, for n>2: a(n) =
 *            48·2^(2k) + 33·2^k – 3 k=(n+4)/4 n ≡ 1 (mod 4): a(n) = 96·2^(2k)
 *            – 30·2^k where k = (n+3)/4 n ≡ 2 (mod 4): a(n) = 96·2^(2k) +
 *            6·2^k – 3 k=(n+2)/4
 * Range:     u8 n <= 1, u16 n <= 16, u32 n <= 48, u64 n <= 112, u128 n <= 240,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) of OEIS A075253. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)77;
    if (n == 1) return (seq_t)166;
    if (n == 2) return (seq_t)267;

    idx_t mod = n % 4;
    idx_t k, exp;
    seq_t a2k, a1k, res;
    /* choose offset and compute k */
    if (mod == 3) {
        k = (n + 5) / 4;
    } else if (mod == 0) {
        k = (n + 4) / 4;
    } else if (mod == 1) {
        k = (n + 3) / 4;
    } else /* mod == 2 */ {
        k = (n + 2) / 4;
    }
    exp = 2 * k;
    /* compute powers of two */
    a2k = ((seq_t)1) << (unsigned long long)exp;   /* 2^(2k) */
    a1k = ((seq_t)1) << (unsigned long long)k;     /* 2^k */

    switch (mod) {
        case 0:
            res = (seq_t)48 * a2k + (seq_t)33 * a1k - (seq_t)3;
            break;
        case 1:
            res = (seq_t)96 * a2k - (seq_t)30 * a1k;
            break;
        case 2:
            res = (seq_t)96 * a2k + (seq_t)6 * a1k - (seq_t)3;
            break;
        case 3:
        default: /* mod 3 */
            res = (seq_t)48 * a2k - (seq_t)21 * a1k;
            break;
    }
    return res;
}
