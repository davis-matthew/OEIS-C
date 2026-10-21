/*
 * A212067  Number of (w,x,y,z) with all terms in {1,...,n} and w^3 = x*y*z.
 * https://oeis.org/A212067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (w,x,y,z) with w^3 = x*y*z and 1≤w,x,y,z≤n
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 1887, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation enumerates divisors of w³ for each w and counts how many
   ordered factor pairs (x,y) lie within [1,n] with product dividing w³.
   It uses only integer arithmetic and is compatible with C11 and C++17. */

#include <stdint.h>
#include <stdlib.h>

/* Global workspace used during recursion */
static int g_primes[16];          /* primes dividing w */
static int g_exps[16];            /* exponent of each prime in w³ */
static int g_cur_exp[16];         /* exponent of each prime in current divisor d */
static int g_k;                  /* number of distinct primes of w */
static uint64_t g_n;              /* current n (value of the argument) */
static uint64_t g_thr;            /* ceil(w³ / n) for the current w */
static seq_t g_total;             /* accumulated answer for the current call */

/* Count divisors q of the current divisor d (whose exponents are in g_cur_exp)
   that satisfy L ≤ q ≤ g_n, where L = ceil(d / n).  Returns the count. */
static seq_t count_range(int idx, uint64_t q, uint64_t L, uint64_t n)
{
    if (idx == g_k) {
        return (q >= L && q <= n) ? (seq_t)1 : (seq_t)0;
    }
    seq_t count = 0;
    uint64_t q_cur = q;
    for (int e = 0; e <= g_cur_exp[idx]; ++e) {
        if (q_cur > n) break;                 /* prune */
        count += count_range(idx + 1, q_cur, L, n);
        if (e < g_cur_exp[idx]) {
            if (q_cur > n / g_primes[idx]) break; /* avoid overflow further */
            q_cur *= g_primes[idx];
        }
    }
    return count;
}

/* Recursive generation of all divisors of w³.  When a divisor d is found,
   its contribution g(d) is added to g_total. */
static void dfs_div(int idx, uint64_t cur)
{
    if (idx == g_k) {
        if (cur >= g_thr) {
            uint64_t d = cur;
            uint64_t L = (d + g_n - 1) / g_n;      /* ceil(d / n) */
            if (L <= g_n) {
                /* count ordered pairs (x,y) with x*y=d and 1≤x,y≤n */
                seq_t cnt = count_range(0, 1, L, g_n);
                g_total += cnt;
            }
        }
        return;
    }
    uint64_t prod = 1;
    for (int e = 0; e <= g_exps[idx]; ++e) {
        g_cur_exp[idx] = e;
        uint64_t next = cur * prod;
        dfs_div(idx + 1, next);
        if (e < g_exps[idx]) {
            if (prod > UINT64_MAX / g_primes[idx]) break;  /* safety */
            prod *= g_primes[idx];
        }
    }
}

/* Main function requested by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    int N = (int)n;
    /* Simple smallest-prime-factor sieve up to N */
    int *spf = (int*)malloc((N + 1) * sizeof(int));
    for (int i = 0; i <= N; ++i) spf[i] = i;
    for (int i = 2; i * i <= N; ++i) {
        if (spf[i] == i) {
            for (int j = i * i; j <= N; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }

    g_n = n;
    g_total = 0;

    for (int w = 1; w <= N; ++w) {
        /* Factorization of w */
        int temp = w;
        int pcnt = 0;
        while (temp > 1) {
            int p = spf[temp];
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            g_primes[pcnt] = p;
            g_exps[pcnt] = e * 3;          /* exponent in w³ */
            ++pcnt;
        }
        g_k = pcnt;

        /* Compute threshold thr = ceil(w³ / n) */
        seq_t w3 = (seq_t)w;
        w3 = w3 * w * w;                 /* w³ fits in 64 bits for n≤10000 */
        seq_t thr_seq = (w3 + n - 1) / n;
        g_thr = (uint64_t)thr_seq;

        /* Enumerate all divisors of w³ */
        dfs_div(0, 1);
    }

    free(spf);
    return g_total;
}
