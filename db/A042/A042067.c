/*
 * A042067  Denominators of continued fraction convergents to sqrt(557).
 * https://oeis.org/A042067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   Denominators of continued–fraction convergents to sqrt(557)
 *            Recurrence: q_{-2}=1, q_{-1}=0; q_k = a_k*q_{k-1} + q_{k-2}
 *            where a_0 = floor(sqrt(557)) and the subsequent a_k are the
 *            periodic continued‑fraction terms of sqrt(557)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---------- Constants for the radicand sqrt(557) ---------- */
static const unsigned long long RADICAND = 557ULL;
static const unsigned long long A0_ll   = 23ULL;   /* floor(sqrt(557)) */

/* ---------- Periodic continued–fraction terms a_1 … a_L ---------- */
static seq_t   *period_terms = NULL;   /* will hold a_1 .. a_L */
static idx_t   period_len   = 0;        /* length L of the period */

/* ---------- Cached denominators a(0)…a(n) ---------- */
static seq_t   *denoms       = NULL;
static idx_t   denom_cap    = 0;        /* current capacity of denoms array */

/* ---------- Recurrence state for incremental evaluation ---------- */
static seq_t p_prev2 = (seq_t)0;        /* p_{k-2}   */
static seq_t p_prev1 = (seq_t)1;        /* p_{k-1}   */
static seq_t q_prev2 = (seq_t)1;        /* q_{k-2}   */
static seq_t q_prev1 = (seq_t)0;        /* q_{k-1}   */
static idx_t  cached_n   = (idx_t)-1;   /* highest n that has been computed */

/* ---------- Utility: initialize periodic sequence (once) ---------- */
static void init_period(void)
{
    if (period_len != 0)
        return;                     /* already initialized */

    uint64_t m = 0, d = 1, a = A0_ll, next_a;
    idx_t   len = 0;
    seq_t *tmp = NULL;
    /* generate the periodic part until the term 2*A0_ll appears */
    do {
        m   = d * a - m;
        d   = (RADICAND - m * m) / d;
        next_a = (A0_ll + m) / d;

        tmp = (seq_t*)realloc(tmp, (len + 1) * sizeof(seq_t));
        if (!tmp) return;           /* allocation failed – caller will fail */
        tmp[len++] = (seq_t)next_a;

        a = next_a;
    } while (next_a != 2 * A0_ll);

    period_terms = tmp;
    period_len   = len;
}

/* ---------- Utility: ensure the denoms array can hold indices up to 'needed' ---------- */
static void ensure_den_cap(idx_t needed)
{
    if (needed <= denom_cap)
        return;
    idx_t new_cap = denom_cap ? denom_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_ptr = (seq_t*)realloc(denoms, new_cap * sizeof(seq_t));
    if (!new_ptr) return;           /* allocation failed – caller will fail */
    denoms = new_ptr;
    denom_cap = new_cap;
}

/* ---------- Main function: a(n) ---------- */
seq_t a(idx_t n)
{
    /* Lazily set up the periodic part */
    init_period();

    /* Make space for needed terms */
    ensure_den_cap(n + 1);

    /* Return cached value if already computed */
    if (cached_n != (idx_t)-1 && n <= cached_n)
        return denoms[n];

    /* Determine the starting index for new computations */
    idx_t k_start = (cached_n == (idx_t)-1) ? 0 : cached_n + 1;

    /* Extend the computation up to index n */
    for (idx_t k = k_start; k <= n; ++k) {
        seq_t ak;
        if (k == 0)               /* a_0 */
            ak = (seq_t)A0_ll;
        else                       /* periodic terms a_1, a_2, ... */
            ak = period_terms[(k - 1) % period_len];

        /* Compute next convergent in the continued‑fraction recurrence */
        seq_t pk = ak * p_prev1 + p_prev2;
        seq_t qk = ak * q_prev1 + q_prev2;

        /* Store the denominator */
        denoms[k] = qk;

        /* Update state for the next iteration */
        p_prev2 = p_prev1;  p_prev1 = pk;
        q_prev2 = q_prev1;  q_prev1 = qk;
    }

    cached_n = n;
    return denoms[n];
}
