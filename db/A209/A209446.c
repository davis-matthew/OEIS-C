/*
 * A209446  a(n) = Pell(n)*A004016(n) for n >= 1, with a(0)=1, where
 *          A004016(n) is the number of integer solutions (x,y) to x^2 + x*y +
 *          y^2 = n.
 * https://oeis.org/A209446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Pell(n) * A004016(n) for n >= 1, with a(0) = 1.
 *            A004016(n) = 6 * (d1(n) - d2(n)), where d1/d2 are divisor counts
 *            ≡ 1 or 2 (mod 3)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 24, u64 n <= 48, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ===== Static storage for Pell numbers ===== */
static seq_t *pell_cache = NULL;      /* array of computed Pell numbers */
static idx_t  pell_max  = 0;          /* highest index computed so far   */
static idx_t  pell_cap  = 0;          /* current allocated capacity     */

/* Ensure Pell numbers up to index n are computed */
static void ensure_pell_upto(idx_t n)
{
    if (n <= pell_max) return;

    /* Allocate/reallocate array large enough for n+1 elements */
    idx_t new_cap = pell_cap ? pell_cap : n + 1;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(pell_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);     /* allocation failed */
    pell_cache = tmp;
    pell_cap   = new_cap;

    /* Initialise base cases if not yet set */
    if (pell_max < 1) {
        pell_cache[0] = (seq_t)0;
        pell_cache[1] = (seq_t)1;
        pell_max = 1;
    }

    /* Compute up to n */
    for (idx_t i = pell_max + 1; i <= n; ++i) {
        pell_cache[i] = pell_cache[i - 1] * (seq_t)2 + pell_cache[i - 2];
    }
    pell_max = n;
}

/* ===== Global state for divisor recursion ===== */
static uint64_t g_primes[16];
static int      g_exps  [16];
static size_t   g_cnt   = 0;
static int      g_diff  = 0;

static void g_dfs_div(size_t idx, idx_t cur)
{
    if (idx == g_cnt) {
        int r = cur % 3;
        if (r == 1) g_diff++;
        else if (r == 2) g_diff--;
        return;
    }

    uint64_t p = g_primes[idx];
    int      e = g_exps[idx];
    idx_t    val = 1;

    for (int i = 0; i <= e; ++i) {
        g_dfs_div(idx + 1, cur * val);
        val *= p;
    }
}

/* Compute d1(n) - d2(n) for A004016(n) */
static int diff_d1d2(idx_t n)
{
    idx_t tmp = n;
    g_cnt = 0;

    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            g_primes[g_cnt] = p;
            g_exps[g_cnt++] = e;
        }
    }
    if (tmp > 1) {          /* remaining prime factor */
        g_primes[g_cnt] = tmp;
        g_exps[g_cnt++] = 1;
    }

    g_diff = 0;
    g_dfs_div(0, 1);
    return g_diff;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* special case */

    ensure_pell_upto(n);

    int diff = diff_d1d2(n);
    if (diff == 0) return (seq_t)0;

    /* a(n) = Pell(n) * 6 * diff */
    seq_t res = pell_cache[n] * (seq_t)(diff * 6);
    return res;
}
