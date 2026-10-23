/*
 * A188171  The number of divisors d of n of the form d == 5 (mod 8).
 * https://oeis.org/A188171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of divisors of n that are ≡ 5 (mod 8) */
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t temp = n;

    /* cnt[r] : number of divisors found so far with residue r mod 8 */
    seq_t cnt[8] = {0}, newcnt[8];
    cnt[1] = 1;               /* divisor 1 */

    /* Handle factor 2 */
    if (temp % 2 == 0) {
        unsigned e = 0;
        while (temp % 2 == 0) { temp /= 2; ++e; }

        for (int r = 0; r < 8; ++r) newcnt[r] = 0;
        uint8_t p_mod8 = 2 % 8;
        uint8_t curpow = 1;          /* 2^0 mod 8 */
        for (unsigned k = 0; k <= e; ++k) {
            for (int r = 0; r < 8; ++r)
                if (cnt[r]) {
                    int res = (r * curpow) & 7;
                    newcnt[res] += cnt[r];
                }
            curpow = (curpow * p_mod8) & 7;
        }
        for (int r = 0; r < 8; ++r) cnt[r] = newcnt[r];
    }

    /* Process odd prime factors by trial division */
    idx_t p = 3;
    while (p <= temp / p) {            /* p^2 <= temp */
        if (temp % p == 0) {
            unsigned e = 0;
            while (temp % p == 0) { temp /= p; ++e; }

            for (int r = 0; r < 8; ++r) newcnt[r] = 0;
            uint8_t p_mod8 = (uint8_t)(p % 8);
            uint8_t curpow = 1;        /* p^0 mod 8 */
            for (unsigned k = 0; k <= e; ++k) {
                for (int r = 0; r < 8; ++r)
                    if (cnt[r]) {
                        int res = (r * curpow) & 7;
                        newcnt[res] += cnt[r];
                    }
                curpow = (curpow * p_mod8) & 7;
            }
            for (int r = 0; r < 8; ++r) cnt[r] = newcnt[r];
        }
        p += 2;
    }

    /* Remaining prime factor (if any) */
    if (temp > 1) {
        idx_t p = temp;                /* prime factor */
        unsigned e = 1;                /* exponent is 1 */
        for (int r = 0; r < 8; ++r) newcnt[r] = 0;
        uint8_t p_mod8 = (uint8_t)(p % 8);
        uint8_t curpow = 1;            /* p^0 mod 8 */
        for (unsigned k = 0; k <= e; ++k) {
            for (int r = 0; r < 8; ++r)
                if (cnt[r]) {
                    int res = (r * curpow) & 7;
                    newcnt[res] += cnt[r];
                }
            curpow = (curpow * p_mod8) & 7;
        }
        for (int r = 0; r < 8; ++r) cnt[r] = newcnt[r];
    }

    return cnt[5];
}
