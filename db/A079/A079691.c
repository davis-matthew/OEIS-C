/*
 * A079691  Number of 6's in n!.
 * https://oeis.org/A079691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = number of digits '6' in the decimal expansion of n!
 * Range:     u8 n <= 1015, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the decimal representation of n! incrementally
   using base‑10 digits.  For each new n it multiplies the current value
   by n, updates the digit vector, counts the 6's, and caches the result. */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static state for incremental factorial evaluation */
    static uint8_t *fac = NULL;      /* decimal digits (least significant first) */
    static size_t fac_len = 0;       /* number of used digits */
    static size_t fac_cap = 0;       /* allocated capacity for digits */

    static seq_t *memo = NULL;      /* cached a(k) values */
    static size_t memo_cap = 0;      /* capacity of memo array */

    static idx_t cur_n = 1;          /* current factorial computed (pre‑filled 1!) */
    static idx_t max_n = 1;          /* highest n for which a(k) is known */

    /* initialize static buffers on first call */
    if (memo == NULL) {
        memo_cap = 128;
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        /* 0! = 1 and 1! = 1 -> no digit '6' */
        memo[0] = 0;
        memo[1] = 0;

        fac_cap = 64;
        fac = (uint8_t *)malloc(fac_cap * sizeof(uint8_t));
        fac_len = 1;
        fac[0] = 1;                  /* factorial of 1 */
    }

    /* extend memo buffer if needed */
    if ((size_t)n + 1 > memo_cap) {
        size_t newcap = memo_cap * 2;
        if (newcap <= (size_t)n + 1) newcap = (size_t)n + 1;
        memo = (seq_t *)realloc(memo, newcap * sizeof(seq_t));
        memo_cap = newcap;
    }

    /* if n already computed, return cached value */
    if (n <= max_n) return memo[n];

    /* compute factorial up to n incrementally */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        /* multiply current factorial by k */
        unsigned long long carry = 0;
        for (size_t i = 0; i < fac_len; ++i) {
            unsigned long long prod = (unsigned long long)fac[i] * (unsigned long long)k + carry;
            fac[i] = (uint8_t)(prod % 10);
            carry   = prod / 10;
        }
        while (carry > 0) {
            if (fac_len == fac_cap) {
                fac_cap = fac_cap * 2 + 64;
                fac = (uint8_t *)realloc(fac, fac_cap * sizeof(uint8_t));
            }
            fac[fac_len++] = (uint8_t)(carry % 10);
            carry /= 10;
        }

        /* count digits '6' in current factorial */
        uint32_t cnt6 = 0;
        for (size_t i = 0; i < fac_len; ++i) {
            if (fac[i] == 6) ++cnt6;
        }
        memo[k] = (seq_t)cnt6;
    }

    max_n = n;
    return memo[n];
}
