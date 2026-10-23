/*
 * A117889  Period of sequence {Kronecker(-n,k), k = 1, 2, 3, ...}, or 0 if
 *          the sequence is not periodic.
 * https://oeis.org/A117889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*rad(n) if 2-adic valuation t of n is odd, rad(n) if t
 *            is even and odd part s ≡ 3 (mod 4), 2*rad(n) if t is even >0 and
 *            s ≡ 1 (mod 4), 0 if t = 0 and s ≡ 1 (mod 4); where rad(n) is the
 *            product of distinct prime factors of n
 * Range:     u8 n <= 65, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute exponent of 2 (t) and odd part (s) */
    uint64_t nn = (uint64_t)n;
    uint64_t t = 0;
    while ((nn & 1ULL) == 0ULL) {
        t++;
        nn >>= 1;
    }
    uint64_t s = nn;              /* odd part */

    /* Compute rad(n) = product of distinct prime factors */
    seq_t rad = 1;
    if (t > 0) rad *= (seq_t)2;   /* 2 divides n */
    uint64_t temp = s;
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            rad *= (seq_t)p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) rad *= (seq_t)temp;   /* remaining prime factor */

    /* Apply the formula for a(n) */
    seq_t res;
    if (t == 0) {
        /* n is odd */
        if (s % 4ULL == 3ULL) res = rad;        /* n ≡ 3 (mod 4) */
        else res = (seq_t)0;                    /* n ≡ 1 (mod 4) */
    } else {
        /* n is even */
        if (t & 1ULL) {                        /* t odd */
            res = rad * ((seq_t)4);
        } else {                               /* t even > 0 */
            if (s % 4ULL == 3ULL) res = rad;    /* s ≡ 3 (mod 4) */
            else res = rad * ((seq_t)2);        /* s ≡ 1 (mod 4) */
        }
    }
    return res;
}
