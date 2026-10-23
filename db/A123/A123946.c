/*
 * A123946  a(3n) = floor(43*2^n/28) - 1, a(3n+1) = a(3n) + 3*2^(n-3), a(3n+2)
 *          = floor(17*2^n/7 - 6/7) for n>=3.
 * https://oeis.org/A123946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 46, u32 n <= 94, u64 n <= 190,
 *            u128 n <= 382, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A123946
 * a(3n)   = floor(43*2^n/28) - 1
 * a(3n+1) = a(3n) + 3*2^(n-3)
 * a(3n+2) = floor((17*2^n - 6)/7)
 * Initial values: a(0)=1, a(1)=1, a(2)=2.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the very first terms explicitly (the compact formulas
       only hold for n ≥ 9, i.e. n = 3·k with k ≥ 3). */
    if (n < 9) {
        switch ((idx_t)n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)1;
        case 2: return (seq_t)2;
        case 3: return (seq_t)2;
        case 4: return (seq_t)3;
        case 5: return (seq_t)4;
        case 6: return (seq_t)5;
        case 7: return (seq_t)7;
        case 8: return (seq_t)9;
        }
    }

    uint64_t k = n / 3;            /* n = 3·k + r */
    int r = (int)(n % 3);

    seq_t pow_k = pow2(k);          /* 2^k */

    if (r == 0) {                  /* a(3k) */
        /* floor(43*pow_k/28) */
        seq_t q = pow_k / 28;
        seq_t rmod = pow_k % 28;
        seq_t val = q * 43 + (rmod * 43) / 28;
        return val - (seq_t)1;
    }

    if (r == 1) {                  /* a(3k+1) = a(3k) + 3*2^(k-3) */
        /* compute a(3k) */
        seq_t q = pow_k / 28;
        seq_t rmod = pow_k % 28;
        seq_t a3k = q * 43 + (rmod * 43) / 28 - (seq_t)1;

        /* add 3*2^(k-3) */
        seq_t pow_k_minus3 = pow2(k - 3);
        return a3k + (seq_t)3 * pow_k_minus3;
    }

    /* r == 2: a(3k+2) = floor((17*pow_k - 6)/7) */
    seq_t q = pow_k / 7;
    seq_t rmod = pow_k % 7;
    seq_t val = q * 17 + ((rmod * 17) - (seq_t)6) / (seq_t)7;
    return val;
}
