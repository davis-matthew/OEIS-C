/*
 * A032275  Number of bracelets (turnover necklaces) of n beads of 4 colors.
 * https://oeis.org/A032275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4^floor((n+1)/2) + 4^ceil((n+1)/2))/4 + (1/(2*n)) *
 *            Σ_{i=1..n} 4^gcd(n,i)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-03)
 * Helpers:   gcd_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd_i32.h"
#include "../utils/mem.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n=0, return 0 safely */

    /* Precompute powers of 4 up to exponent n */
    seq_t *pow4 = alloc_seq_array(n + 1);
    pow4[0] = 1;
    for (idx_t e = 1; e <= n; ++e) {
        pow4[e] = mul_small(pow4[e - 1], 4);
    }

    /* First part: (4^floor((n+1)/2) + 4^ceil((n+1)/2))/4 */
    idx_t floor_exp = (n + 1) / 2;      /* floor((n+1)/2) */
    idx_t ceil_exp  = (n + 2) / 2;      /* ceil((n+1)/2) = floor((n+2)/2) */
    seq_t first_part = pow4[floor_exp] + pow4[ceil_exp];
    first_part /= 4;

    /* Second part: (1/(2*n)) * Σ 4^gcd(n,i)  */
    seq_t sum_div = 0;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t g = (idx_t)gcd_i32((int)n, (int)i);
        sum_div += pow4[g];
    }
    seq_t second_part = sum_div / ( (seq_t)(2 * n) );

    seq_t result = first_part + second_part;

    free_seq_array(pow4);
    return result;
}
