/*
 * A004540  Expansion of sqrt(2) in base 3.
 * https://oeis.org/A004540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = n-th base-3 digit of sqrt(2), i.e. floor(sqrt(2)*3^(n-1))
 *            mod 3, computed by the digit-by-digit (schoolbook) square-root
 *            method in base 3: r <- 9r + (2 on the first step, else 0); d =
 *            max{d in 0..2 : (6p+d)*d <= r}; r <- r - (6p+d)*d; p <- 3p + d;
 *            a(n) = d at step n
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   cmp_limbs, lin, sub_limbs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/cmp_limbs.h"
#include "../utils/lin.h"
#include "../utils/sub_limbs.h"

static uint32_t *P = NULL, *R = NULL, *T = NULL;
static unsigned char *D = NULL;
static size_t cap = 0;     /* limbs */
static idx_t done = 0;     /* digits computed */

seq_t a(idx_t n) {
    if (n > done) {
        idx_t target = n + n / 2 + 64;
        size_t newcap = (size_t)((target + 4) * 1585 / 1000 + 64) / 32 + 4;
        if (newcap > cap) {
            P = (uint32_t *)realloc(P, newcap * sizeof(uint32_t));
            R = (uint32_t *)realloc(R, newcap * sizeof(uint32_t));
            T = (uint32_t *)realloc(T, newcap * sizeof(uint32_t));
            memset(P + cap, 0, (newcap - cap) * sizeof(uint32_t));
            memset(R + cap, 0, (newcap - cap) * sizeof(uint32_t));
            memset(T + cap, 0, (newcap - cap) * sizeof(uint32_t));
            cap = newcap;
        }
        D = (unsigned char *)realloc(D, (size_t)target);
        while (done < target) {
            uint32_t d = 0;
            lin(R, R, 9, done == 0 ? 2u : 0u, cap);
            lin(T, P, 12, 4, cap);              /* (6p+2)*2 */
            if (cmp_limbs(T, R, cap) <= 0) {
                d = 2;
            } else {
                lin(T, P, 6, 1, cap);           /* (6p+1)*1 */
                if (cmp_limbs(T, R, cap) <= 0) d = 1;
            }
            if (d) sub_limbs(R, T, cap);
            lin(P, P, 3, d, cap);
            D[done] = (unsigned char)d;
            done++;
        }
    }
    return (seq_t)D[n - 1];
}
