/*
 * A208280  Number of distinct values in n-th row of Pascal's triangle mod 10
 *          (A008975).
 * https://oeis.org/A208280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct residues of C(n,k) modulo 10 for 0 ≤ k
 *            ≤ n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9053 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Combine residues mod 2 and mod 5 to residue mod 10 via simple CRT */
static uint8_t combine_mod10(uint8_t r2, uint8_t r5)
{
    uint8_t r = r5;
    while ((r & 1U) != r2) r += 5U;
    return r;
}

/* return C(a,b) mod 5 for 0 ≤ a,b ≤ 4; invalid b>a returns 0 */
static inline uint8_t small_binom_mod5(uint8_t a, uint8_t b)
{
    static const uint8_t tab[5][5] = {
        {1,0,0,0,0},
        {1,1,0,0,0},
        {1,2,1,0,0},
        {1,3,3,1,0},
        {1,4,1,4,1}
    };
    return (b > a) ? 0U : tab[a][b];
}

/* binomial coefficient modulo 2: 1 iff (k & (n-k)) == 0, otherwise 0 */
static inline uint8_t binom_mod2(idx_t n, idx_t k)
{
    return ((k & (n - k)) == 0) ? 1U : 0U;
}

/* binomial coefficient modulo 5 using Lucas theorem (base 5) */
static uint8_t binom_mod5(idx_t n, idx_t k)
{
    if (k > n) return 0U;
    uint8_t res = 1U;
    while (n > 0 || k > 0) {
        uint8_t ni = (uint8_t)(n % 5);
        uint8_t ki = (uint8_t)(k % 5);
        if (ki > ni) return 0U;
        res = (uint8_t)((res * small_binom_mod5(ni, ki)) % 5U);
        n /= 5;
        k /= 5;
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    bool seen[10] = {0};
    for (idx_t k = 0; k <= n; ++k) {
        uint8_t r2 = binom_mod2(n, k);
        uint8_t r5 = binom_mod5(n, k);
        uint8_t r  = combine_mod10(r2, r5);
        seen[r] = true;
    }
    seq_t count = 0;
    for (int i = 0; i < 10; ++i)
        if (seen[i]) ++count;
    return count;
}
