/*
 * A036756  Number of strings of n distinct digits from 1-9 that are the last
 *          n digits of a square in base 10.
 * https://oeis.org/A036756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of length‑n strings of distinct digits 1‑9 that occur as the
   last n digits of a perfect square.  The function memoises square
   residue tables for 2ⁿ and 5ⁿ, then enumerates all P(9,n)
   permutations and counts those for which the residue is a quadratic
   residue modulo both 2ⁿ and 5ⁿ.  For n > 9 the count is 0 because
   no nine distinct digits exist. */

#include <stdint.h>
#include <stdlib.h>

static uint64_t pow5[10] = {0};
static uint8_t  *sq2[10] = {0};
static uint8_t  *sq5[10] = {0};

static void init_pow5(void)
{
    pow5[0] = 1;
    for (int i = 1; i < 10; ++i) pow5[i] = pow5[i - 1] * 5;
}

static void build_sq2(int n)
{
    uint64_t m = 1ULL << n;        /* 2^n  */
    sq2[n] = (uint8_t*)malloc(m);
    for (uint64_t i = 0; i < m; ++i) sq2[n][i] = 0;
    for (uint64_t i = 0; i < m; ++i)
        sq2[n][(i * i) % m] = 1;
}

static void build_sq5(int n)
{
    uint64_t m = pow5[n];          /* 5^n  */
    sq5[n] = (uint8_t*)malloc(m);
    for (uint64_t i = 0; i < m; ++i) sq5[n][i] = 0;
    for (uint64_t i = 0; i < m; ++i)
        sq5[n][(i * i) % m] = 1;
}

/* Recursive enumeration of permutations of length n using digits 1–9 */
static void permute(int n, int pos, uint16_t mask, uint64_t val,
                    uint64_t *cnt)
{
    if (pos == n) {
        /* check quadratic residue condition */
        uint64_t mod2 = 1ULL << n;
        uint64_t mod5 = pow5[n];
        if (sq2[n][val % mod2] && sq5[n][val % mod5])
            ++(*cnt);
        return;
    }
    for (int d = 1; d <= 9; ++d) {
        uint16_t bit = 1U << d;
        if (mask & bit) continue;   /* digit already used */
        permute(n, pos + 1, mask | bit, val * 10 + d, cnt);
    }
}

/* -------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1, below irrelevant */
    if (n > 9) return 0;   /* cannot have more than 9 distinct non‑zero digits */

    /* initialise auxiliary tables on first use */
    if (pow5[0] == 0) init_pow5();
    if (!sq2[n]) build_sq2((int)n);
    if (!sq5[n]) build_sq5((int)n);

    uint64_t count = 0;
    permute((int)n, 0, 0, 0, &count);
    return (seq_t)count;
}
