/*
 * A083915  Number of divisors of n that are congruent to 5 modulo 10.
 * https://oeis.org/A083915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors d of n with d ≡ 5 (mod 10)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

static idx_t fac_p[64];
static int   fac_e[64];
static int   fac_cnt;
static seq_t result;

/* recursive enumeration of divisors, counting those ≡5 (mod 10) */
static void dfs(int idx, int cur_mod10)
{
    if (idx == fac_cnt) {
        if (cur_mod10 == 5)
            result++;
        return;
    }
    int pmod = fac_p[idx] % 10;
    int mul  = 1;
    for (int i = 0; i <= fac_e[idx]; i++) {
        dfs(idx + 1, (cur_mod10 * mul) % 10);
        mul = (mul * pmod) % 10;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n < 1, but return 0 safely */

    /* factorise n using trial division */
    idx_t m = n;
    fac_cnt = 0;

    if ((m & 1) == 0) {        /* handle factor 2 */
        int e = 0;
        do {
            m >>= 1;
            ++e;
        } while ((m & 1) == 0);
        fac_p[fac_cnt] = 2;
        fac_e[fac_cnt] = e;
        ++fac_cnt;
    }

    for (idx_t p = 3; (p * p) <= m; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            fac_p[fac_cnt] = p;
            fac_e[fac_cnt] = e;
            ++fac_cnt;
        }
    }
    if (m > 1) {                /* remaining prime factor */
        fac_p[fac_cnt] = m;
        fac_e[fac_cnt] = 1;
        ++fac_cnt;
    }

    /* enumerate all divisors and count those ≡ 5 (mod 10) */
    result = 0;
    dfs(0, 1);                  /* start with divisor 1 (mod 10 = 1) */

    return result;
}
