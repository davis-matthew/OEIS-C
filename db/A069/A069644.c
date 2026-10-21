/*
 * A069644  Let M_n be the n X n matrix with M_n(i,j)=1/(i+j+5); then
 *          a(n)=1/det(M_n).
 * https://oeis.org/A069644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A069644:
   a(n) = 1 / det(M_n),  M_n(i,j) = 1/(i+j+5),  offset 1.
   Determinant formula:
   det(M_n) = ( ∏_{s=7}^{2n+5} s^{c_s} ) / ( ∏_{k=1}^{n-1} k^{2(n-k)} )
   with c_s = t-1   (t = s-5 ≤ n+1)
          c_s = 2n-t+1 (otherwise).
   So a(n) = ∏_{p} p^{e_p} where e_p = ∑_{s} c_s * v_p(s) - ∑_{k} 2(n-k) * v_p(k).   */

#include <stdint.h>
#include <string.h>

/* Maximum n that will be requested (adjust if necessary) */
#define MAX_N        10000
#define MAX_VAL      (2*MAX_N+5)          /* maximal s or k needed */
#define MAX_PRIMES   2500                 /* upper bound on number of primes <= MAX_VAL */

/* Sieve data */
static int spf[MAX_VAL+1];
static int primes[MAX_PRIMES];
static int prime_idx[MAX_VAL+1];
static int prime_count = 0;
static int initialized = 0;

/* Forward declaration of power function */
static seq_t pow_uint(seq_t base, int exp);

/* One‑time initialization of sieve and prime tables */
static void init(void)
{
    if (initialized) return;
    int i, j;
    for (i = 0; i <= MAX_VAL; ++i) spf[i] = 0;
    for (i = 0; i <= MAX_VAL; ++i) prime_idx[i] = -1;

    for (i = 2; i <= MAX_VAL; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            prime_idx[i] = prime_count;
            primes[prime_count++] = i;
            if ((int64_t)i * i <= MAX_VAL) {
                for (j = i * i; j <= MAX_VAL; j += i) {
                    if (spf[j] == 0) spf[j] = i;
                }
            }
        }
    }
    initialized = 1;
}

/* Compute base^exp using repeated squaring (seq_t may be big integer) */
static seq_t pow_uint(seq_t base, int exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init();                      /* ensure sieve ready */
    int N = (int)n;              /* n is small enough to fit in int */
    int exp[MAX_PRIMES] = {0};   /* exponent array for each prime */

    /* Numerator: ∏ s^{c_s}  (s from 7 to 2N+5) */
    for (int s = 7; s <= 2 * N + 5; ++s) {
        int t = s - 5;
        int c = (t <= N + 1) ? (t - 1) : (2 * N - t + 1);
        if (c == 0) continue;

        int x = s;
        while (x > 1) {
            int p = spf[x];
            int cnt = 0;
            while (x % p == 0) { x /= p; ++cnt; }
            exp[prime_idx[p]] += c * cnt;
        }
    }

    /* Denominator: ∏ k^{2(N-k)} (k from 1 to N-1) */
    for (int k = 1; k <= N - 1; ++k) {
        int coeff = 2 * (N - k);
        int x = k;
        while (x > 1) {
            int p = spf[x];
            int cnt = 0;
            while (x % p == 0) { x /= p; ++cnt; }
            exp[prime_idx[p]] -= coeff * cnt;
        }
    }

    /* Build the result from prime powers */
    seq_t result = 1;
    for (int i = 0; i < prime_count; ++i) {
        int e = exp[i];
        if (e > 0) result *= pow_uint((seq_t)primes[i], e);
    }
    return result;
}
