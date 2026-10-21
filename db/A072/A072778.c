/*
 * A072778  Number of powers of squarefree numbers <= n.
 * https://oeis.org/A072778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Q(n) + Σ_{k=2}^{∞} ( Q(⌊n^{1/k}⌋) – 1 ) where Q(x) is the
 *            number of squarefree integers ≤ x
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following code is valid C11 and C++17. */

#include <stdint.h>

/* Maximum sqrt(n) we may need. 1 000 000 gives enough room for n up to 10¹².
   For the usual tests (n ≤ 10 000) this is overkill but harmless. */
#define MU_MAX 1000000

/* Global data for the Möbius function μ(d). */
static int   mu_arr[MU_MAX + 1];
static char  is_prime_arr[MU_MAX + 1];
static int   primes_arr[MU_MAX + 1];
static int   primes_cnt;
static int   mu_limit = 0;

/* Build μ(d) for 1 ≤ d ≤ limit using a linear sieve. */
static void
init_mu(int limit)
{
    if (limit <= mu_limit)
        return;
    if (limit > MU_MAX) /* safety */
        limit = MU_MAX;
    mu_limit = limit;

    for (int i = 0; i <= limit; ++i) {
        is_prime_arr[i] = 1;
        mu_arr[i] = 0;
    }
    is_prime_arr[0] = is_prime_arr[1] = 0;
    mu_arr[1] = 1;
    primes_cnt = 0;

    for (int i = 2; i <= limit; ++i) {
        if (is_prime_arr[i]) {
            primes_arr[primes_cnt++] = i;
            mu_arr[i] = -1;
        }
        for (int j = 0; j < primes_cnt; ++j) {
            int p = primes_arr[j];
            long long ip = (long long)i * p;
            if (ip > limit)
                break;
            is_prime_arr[ip] = 0;
            if (i % p == 0) {
                mu_arr[ip] = 0;
                break;
            } else {
                mu_arr[ip] = -mu_arr[i];
            }
        }
    }
}

/* Integer square‑root by incremental search (safe for 64‑bit). */
static idx_t
int_sqrt(idx_t n)
{
    idx_t r = 0;
    while (((r + 1) * (r + 1)) <= n)
        ++r;
    return r;
}

/* Integer k‑th root floor: maximum r with r^k ≤ n. */
static idx_t
kth_root_floor(idx_t n, idx_t k)
{
    idx_t lo = 1, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __int128 p = 1;
        int ok = 1;
        for (idx_t i = 0; i < k; ++i) {
            p *= mid;
            if (p > n) {
                ok = 0;
                break;
            }
        }
        if (ok)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* Count squarefree integers ≤ x. */
static seq_t
sqf_count(idx_t x)
{
    if (x == 0)
        return 0;
    idx_t lim = int_sqrt(x);
    init_mu(lim);

    seq_t pos = 0;
    seq_t neg = 0;

    for (idx_t d = 1; d <= lim; ++d) {
        int m = mu_arr[d];
        if (m == 1)
            pos += x / (d * d);
        else if (m == -1)
            neg += x / (d * d);
    }
    return pos >= neg ? pos - neg : 0;
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t res = sqf_count(n);   /* count of squarefree numbers ≤ n (includes 1) */

    for (idx_t k = 2; ; ++k) {
        idx_t root = kth_root_floor(n, k);
        if (root < 2)
            break;
        seq_t cnt = sqf_count(root);
        if (cnt > 1)          /* subtract the root = 1 entry */
            res += (cnt - 1);
    }
    return res;
}
