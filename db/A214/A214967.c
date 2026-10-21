/*
 * A214967  a(n) is the least m > 0 such that n-m divides Fibonacci(n)-m.
 * https://oeis.org/A214967
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the least m > 0 such that n-m divides Fib(n)-m.
 *            Computation uses: a(n) = n - d where d is the largest integer 1
 *            ≤ d ≤ n-1 that divides Fib(n)-n. The sequence is precomputed up
 *            to n = 10000 and memoised. For n > 10000 a fast‑doubling
 *            algorithm is used
 * Range:     u8 n <= 257, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define MAX_N 10000   /* precomputation limit */

static seq_t A[MAX_N+1];              /* memoised answers */
static unsigned short f_cur[MAX_N+1]; /* Fib(current_n) mod d   */
static unsigned short f_prev[MAX_N+1];/* Fib(current_n-1) mod d */
static bool initialized = false;

/* fast doubling for Fib(n) modulo a small d (d < 65536) */
static void fib_doubling(idx_t n, unsigned short mod,
                        unsigned short *fn, unsigned short *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1 % mod;
        return;
    }
    unsigned short a, b;
    fib_doubling(n >> 1, mod, &a, &b);
    unsigned long long c = (unsigned long long)a *
              (((2ULL * b) % mod + mod) - a);
    c %= mod;
    unsigned long long d = ((unsigned long long)a * a +
                            (unsigned long long)b * b) % mod;

    if ((n & 1ULL) == 0) {
        *fn  = (unsigned short)c;
        *fn1 = (unsigned short)d;
    } else {
        unsigned long long e = (c + d) % mod;
        *fn  = (unsigned short)d;
        *fn1 = (unsigned short)e;
    }
}

/* Fib(n) mod mod (mod > 0, < 65536) */
static unsigned short fib_mod_u64(idx_t n, unsigned short mod)
{
    unsigned short fn, fn1;
    fib_doubling(n, mod, &fn, &fn1);
    return fn;
}

/* precompute up to MAX_N */
static void init(void)
{
    if (initialized) return;

    /* initial values: Fib(0)=0, Fib(1)=1 */
    for (idx_t d = 1; d <= MAX_N; ++d) {
        f_cur[d]  = 1 % (unsigned short)d;
        f_prev[d] = 0;
    }
    A[0] = 0;
    A[1] = 0;

    for (idx_t n = 2; n <= MAX_N; ++n) {
        /* update Fibonacci residues for all d */
        for (idx_t d = 1; d <= MAX_N; ++d) {
            unsigned short sum = f_cur[d] + f_prev[d];
            if (sum >= (unsigned short)d) sum -= (unsigned short)d;
            f_prev[d] = f_cur[d];
            f_cur[d]  = sum;
        }

        /* find largest d (1 ≤ d ≤ n-1) that divides Fib(n)-n */
        idx_t best_d = 1;  /* d = 1 always works because mod 1 == 0 */
        for (int d = (int)(n-1); d >= 1; --d) {
            unsigned short fm   = f_cur[d];
            unsigned short nmod = (unsigned short)(n % (idx_t)d);
            unsigned short gmod;
            if (fm < nmod) gmod = fm + d - nmod;
            else           gmod = fm - nmod;
            if (gmod == 0) { best_d = (idx_t)d; break; }
        }
        A[n] = (seq_t)(n - best_d);
    }
    initialized = true;
}

/* public entry point */
seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* undefined for n < 2, but return 0 */

    if (!initialized) init();

    if (n <= MAX_N) return A[n];

    /* n > MAX_N: compute on the fly using fast doubling */
    seq_t result = 0;
    for (int d = (int)(n-1); d >= 1; --d) {
        unsigned short mod = (unsigned short)d;
        unsigned short fm  = fib_mod_u64(n, mod);
        unsigned short nmod = (unsigned short)(n % (idx_t)d);
        unsigned short gmod;
        if (fm < nmod) gmod = fm + d - nmod;
        else           gmod = fm - nmod;
        if (gmod == 0) { result = n - d; break; }
    }
    return result;
}
