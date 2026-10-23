/*
 * A014869  Numbers k that divide s(k), where s(1)=1, s(j)=19*s(j-1)+j.
 * https://oeis.org/A014869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers k that divide s(k) where s(1)=1, s(j)=19*s(j-1)+j
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   mod_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the nth such k (offset 1). */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mod_mul.h"        /* for mod_mul(uint64_t a,b,mod) */
#include "../utils/seq_realloc.h"    /* for seq_realloc(seq_t*,size_t,size_t) */

/* Recursive helper that returns (p,f,g) for given n:
   p = r^n mod mod
   f = sum_{t=0}^{n-1} r^t mod mod
   g = sum_{t=0}^{n-1} t * r^t mod mod
*/
typedef struct {
    uint64_t p;
    uint64_t f;
    uint64_t g;
} res_t;

static res_t calc_t(uint64_t n, uint64_t r, uint64_t mod)
{
    if (n == 0) {
        /* r^0 = 1 mod mod (handling mod==1 case) */
        res_t res = { (mod == 1 ? 0 : 1 % mod), 0, 0 };
        return res;
    }
    if ((n & 1) == 0) {                    /* even n = 2*m */
        uint64_t m = n >> 1;
        res_t half = calc_t(m, r, mod);

        uint64_t p = mod_mul(half.p, half.p, mod);

        uint64_t f = half.f + mod_mul(half.p, half.f, mod);
        if (f >= mod) f -= mod;

        /* g = g*(1+p) + m*p*f  (all modulo mod) */
        uint64_t g1 = half.g + mod_mul(half.p, half.g, mod);
        if (g1 >= mod) g1 -= mod;

        uint64_t mmod = m % mod;
        uint64_t tmp = mod_mul(mmod, half.p, mod);
        tmp = mod_mul(tmp, half.f, mod);

        uint64_t g = g1 + tmp;
        if (g >= mod) g -= mod;

        return (res_t){p, f, g};
    } else {                                /* odd n = 2*m+1 */
        res_t prev = calc_t(n - 1, r, mod);
        uint64_t p = mod_mul(prev.p, r, mod);
        uint64_t f = prev.f + prev.p;
        if (f >= mod) f -= mod;

        uint64_t nm = (n - 1) % mod;
        uint64_t g = prev.g + mod_mul(nm, prev.p, mod);
        if (g >= mod) g -= mod;

        return (res_t){p, f, g};
    }
}

static uint64_t compute_T(uint64_t n, uint64_t r, uint64_t mod)
{
    /* returns sum_{t=0}^{n-1} t * r^t (mod mod) */
    return calc_t(n, r, mod).g;
}

seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static seq_t *table = NULL;
    static size_t tbl_len = 0;
    static size_t tbl_cap = 0;
    static idx_t next_k = 1;           /* next candidate k to test */

    while (tbl_len < (size_t)n) {
        uint64_t k = (uint64_t)next_k;
        uint64_t r = 19 % k;
        uint64_t t = compute_T(k, r, k);  /* 0 <= t < k */

        if (t == 0) {                     /* k divides s(k) */
            if (tbl_len == tbl_cap) {
                size_t new_cap = tbl_cap ? tbl_cap * 2 : 16;
                seq_t *tmp = seq_realloc(table, tbl_len, new_cap);
                table = tmp;
                tbl_cap = new_cap;
            }
            table[tbl_len++] = (seq_t)k;
        }

        next_k++;
    }

    return table[n - 1];
}
