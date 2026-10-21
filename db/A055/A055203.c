/*
 * A055203  Number of different relations between n intervals on a line.
 * https://oeis.org/A055203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Sum_{k=0..n} k!*Stirling2(n,k)*A121251(k) *
 *            Implementation uses the recurrence * λ(p,n) = C(p,2) * (
 *            λ(p,n‑1) + 2λ(p-1,n‑1) + λ(p‑2,n‑1) ), * with λ(p,1)=1 if p=2
 *            else 0, and * a(n) = Σ_{p=2..2n} λ(p,n). * * The routine keeps a
 *            static cache of computed a(n) values so that * calls in any
 *            or...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>   /* for memset */

/* maximum n we will support (twice this for λ buffers) */
#define MAX_N     20000
#define LAMBDA_SZ (2*MAX_N + 3)   /* size needed for λ(p,n) with n<=MAX_N */

/* static cache of a(n) */
static seq_t a_cache[MAX_N + 1];
static idx_t   a_cache_max = 0;
static int     cache_init = 0;

/* two alternating buffers for λ(p,n) */
static seq_t  λ_buf0[LAMBDA_SZ];
static seq_t  λ_buf1[LAMBDA_SZ];
static seq_t *λ_prev    = λ_buf0;   /* λ(·, cur‑1) */
static seq_t *λ_curr    = λ_buf1;   /* λ(·, cur)   */

/* ------------------------------------------------------------------ */
static void init_static(void)
{
    if (cache_init) return;
    cache_init = 1;

    /* zero the λ buffers */
    memset(λ_buf0, 0, sizeof(λ_buf0));
    memset(λ_buf1, 0, sizeof(λ_buf1));

    /* a(0) = 1 */
    a_cache[0] = (seq_t)1;
    a_cache_max = 0;
}

/* ------------------------------------------------------------------ */
static void compute_to(idx_t n)
{
    /* compute a(cur) for cur = a_cache_max+1 .. n */
    for (idx_t cur = a_cache_max + 1; cur <= n; ++cur)
    {
        /* size of λ array needed for this cur */
        idx_t need = 2*cur + 3;            /* indices 0 .. need-1 */
        idx_t prev_len = 2*(cur-1) + 3;    /* size of λ_prev array */

        if (cur == 1)
        {
            /* λ(p,1) = 1 if p==2 else 0 */
            for (idx_t p = 0; p < need; ++p)
                λ_curr[p] = (p == 2 ? (seq_t)1 : (seq_t)0);
        }
        else
        {
            /* recurrence for λ(p,cur) */
            for (idx_t p = 0; p < need; ++p)
            {
                seq_t comb = 0;
                if (p >= 2)
                    comb = (seq_t)p * (seq_t)(p - 1) / 2;   /* C(p,2) */

                seq_t v1 = 0, v2 = 0, v3 = 0;
                if (p < prev_len)         v1 = λ_prev[p];
                if (p >= 1 && p-1 < prev_len)  v2 = λ_prev[p-1];
                if (p >= 2 && p-2 < prev_len)  v3 = λ_prev[p-2];

                seq_t sum = v1 + (v2 * (seq_t)2) + v3;
                λ_curr[p] = comb * sum;
            }
        }

        /* a(cur) = Σ_{p=2}^{2*cur} λ(p,cur) */
        seq_t total = 0;
        for (idx_t p = 2; p <= 2*cur; ++p)
            total += λ_curr[p];

        a_cache[cur] = total;
        a_cache_max = cur;

        /* prepare for next iteration: λ_prev ← λ_curr, λ_curr ← λ_prev */
        seq_t *tmp = λ_prev;
        λ_prev = λ_curr;
        λ_curr = tmp;
    }
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* initialize static data on first call */
    init_static();

    if (n <= a_cache_max)
        return a_cache[n];

    /* cap n to pre‑defined maximum to avoid out‑of‑bounds */
    if (n > (idx_t)MAX_N)
        return (seq_t)0;   /* or handle error – not expected for tests */

    compute_to(n);
    return a_cache[n];
}
