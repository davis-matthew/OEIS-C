/*
 * A071957  Sum( i=1, n, K(i,n) ) where K(x,y) is the Kronecker symbol (x/y).
 * https://oeis.org/A071957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{i=1}^{n} K(i,n) where K is the Kronecker symbol (i/n)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   legendre_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm factors n, then evaluates K(i,n) for i = 1 .. n and
   accumulates the sum.    */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/legendre_mod.h"

/* Precomputed list of primes up to 10000 */
static int prime_list[2000];
static int prime_count = 0;

static void init_primes(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    const int MAXP = 10000;
    bool is_prime[MAXP + 1];
    for (int i = 0; i <= MAXP; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= MAXP; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= MAXP; j += i)
                is_prime[j] = false;
        }
    }
    for (int i = 2; i <= MAXP; ++i)
        if (is_prime[i]) prime_list[prime_count++] = i;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_primes();

    /* Factor n */
    unsigned long long tn = n;
    int e2 = 0;
    int fpr[8];   /* odd prime factors (max 8 distinct primes for n≤10000) */
    int fexp[8];
    int fcnt = 0;

    for (int i = 0; i < prime_count && (unsigned long long)prime_list[i] * prime_list[i] <= tn; ++i) {
        int p = prime_list[i];
        if (tn % p == 0) {
            int cnt = 0;
            do {
                tn /= p;
                ++cnt;
            } while (tn % p == 0);
            if (p == 2)
                e2 = cnt;
            else {
                fpr[fcnt] = p;
                fexp[fcnt] = cnt;
                ++fcnt;
            }
        }
    }
    if (tn > 1) {               /* remaining prime factor */
        if ((int)tn == 2)
            e2 = 1;
        else {
            fpr[fcnt] = (int)tn;
            fexp[fcnt] = 1;
            ++fcnt;
        }
    }

    /* Sum over i = 1 .. n */
    long long sum = 0;
    for (unsigned long long i = 1; i <= n; ++i) {
        int sign = 1;          /* 1, -1 or 0 (0 indicates cancellation) */

        /* Handle factor 2^e2 */
        if (e2 > 0) {
            if ((i & 1ULL) == 0ULL) {      /* i is even => factor zero */
                sign = 0;
            } else {
                int mod8 = (int)(i & 7ULL);
                int s2;
                if (mod8 == 1 || mod8 == 7) s2 = 1;
                else if (mod8 == 3 || mod8 == 5) s2 = -1;
                else s2 = 1; /* should not occur for odd i */
                if ((e2 & 1) == 1)
                    sign *= s2;              /* odd exponent of 2 */
            }
        }
        if (sign == 0) continue;

        /* Handle odd prime factors */
        for (int j = 0; j < fcnt; ++j) {
            int p = fpr[j];
            int k = fexp[j];
            if ((k & 1) == 0) {             /* even exponent: zero if i divisible by p */
                if ((i % (unsigned long long)p) == 0ULL) {
                    sign = 0;
                    break;
                }
            } else {                         /* odd exponent: Legendre symbol */
                int leg = legendre_mod(i, p);
                if (leg == 0) {
                    sign = 0;
                    break;
                }
                sign *= leg;
            }
        }
        if (sign == 0) continue;
        sum += sign;
    }

    return (seq_t)sum;
}
