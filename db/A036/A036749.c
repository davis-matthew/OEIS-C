/*
 * A036749  Number of strings of n distinct digits from 0-9 that are the last
 *          n digits of a square in base 7.
 * https://oeis.org/A036749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts the number of n‑digit base‑7 strings with distinct digits
 * that are the last n digits of a square in base 7.
 *
 * For n ≤ 7, the number of such strings is computed by enumerating
 * all permutations of n distinct digits from {0,…,6} and testing
 * whether the corresponding residue modulo 7ⁿ is a quadratic residue.
 *
 * For n > 7 there are no such strings (since only seven distinct
 * digits exist), hence a(n) = 0 for all n ≥ 8.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* powers of 7 up to 7⁷ (fits in 32 bits for n ≤ 7) */
static const uint64_t pow7[8] = {
    1ULL,        /* 7⁰ */
    7ULL,        /* 7¹ */
    49ULL,       /* 7² */
    343ULL,      /* 7³ */
    2401ULL,     /* 7⁴ */
    16807ULL,    /* 7⁵ */
    117649ULL,   /* 7⁶ */
    823543ULL    /* 7⁷ */
};

/* memoisation */
static seq_t memo[8];
static bool computed[8] = { false };

/* depth‑first search for permutations of distinct base‑7 digits 0…6 */
static void dfs(size_t n, size_t pos, uint8_t used,
                uint64_t curr, uint64_t P,
                const uint8_t *sqr, size_t *cnt)
{
    if (pos == n) {
        if (sqr[curr])
            ++(*cnt);
        return;
    }
    for (uint8_t d = 0; d < 7; ++d) {
        if (!(used & (1 << d))) {
            uint64_t next = curr * 7ULL + d;
            /* next < P for all n ≤ 7, but take modulo just in case */
            if (next >= P)
                next %= P;
            dfs(n, pos + 1, used | (1 << d), next, P, sqr, cnt);
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1 || n > 7)          /* no distinct 7‑digit strings for n > 7 */
        return (seq_t)0;

    if (computed[n])
        return memo[n];

    uint64_t P = pow7[(uint64_t)n];   /* 7ⁿ, < 2³² */
    /* allocate array of size P for marking quadratic residues */
    uint8_t *sqr = (uint8_t *)calloc((size_t)P, 1);
    if (sqr == NULL)          /* allocation failure – return 0 */
        return (seq_t)0;

    /* mark all quadratic residues modulo P */
    for (uint32_t m = 0; m < (uint32_t)P; ++m) {
        uint64_t r = ((uint64_t)m * m) % P;  /* avoid 32‑bit overflow */
        sqr[r] = 1;
    }

    size_t cnt = 0;
    dfs((size_t)n, 0, 0, 0, P, sqr, &cnt);

    free(sqr);
    memo[n] = (seq_t)cnt;
    computed[n] = true;
    return memo[n];
}
