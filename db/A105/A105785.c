/*
 * A105785  Number of different forests of rooted trees, without isolated
 *          vertices, on n labeled nodes.
 * https://oeis.org/A105785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..n-1} binom(n-1,j) * (j+1)^j * a(n-1-j) with
 *            a(0)=1, a(1)=0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


/* Exponentiation by squaring for seq_t.  Works for arbitrary‑precision integer types as well. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1) result = result * base;
        if (exp > 1) base = base * base;
        exp >>= 1;
    }
    return result;
}

/* Dynamic arrays for cached values of a(n) and of (j+1)^j.  */
static seq_t *seq_vals = NULL;   /* a(n) values */
static idx_t seq_cap  = 0;
static idx_t seq_len  = 0;

static seq_t *pow_vals = NULL;   /* pow_vals[j] = (j+1)^j, j>=0 */
static idx_t pow_cap  = 0;
static idx_t pow_len  = 0;

/* Ensure seq_vals has at least 'needed+1' elements.  Returned pointer is in seq_vals. */
static void ensure_seq_cap(idx_t needed)
{
    if (needed < seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap <= (size_t)needed) new_cap *= 2;
    seq_vals = (seq_t*)realloc(seq_vals, new_cap * sizeof(seq_t));
    seq_cap  = (idx_t)new_cap;
}

/* Ensure pow_vals has at least 'needed+1' elements. */
static void ensure_pow_cap(idx_t needed)
{
    if (needed < pow_cap) return;
    size_t new_cap = pow_cap ? pow_cap * 2 : 8;
    while (new_cap <= (size_t)needed) new_cap *= 2;
    pow_vals = (seq_t*)realloc(pow_vals, new_cap * sizeof(seq_t));
    pow_cap  = (idx_t)new_cap;
}

/* Precompute all (j+1)^j for j up to 'n'.  */
static void precompute_powers(idx_t n)
{
    if (n == 0) return;
    ensure_pow_cap(n);
    for (idx_t j = pow_len; j <= n; ++j)
        pow_vals[j] = pow_seq((seq_t)(j + 1), j);
    pow_len = n + 1;
}

/* Compute a(n) for all n up to the requested value and cache the results. */
static void compute_up_to(idx_t n)
{
    if (n < seq_len) return;       /* already known */
    precompute_powers(n);

    ensure_seq_cap(n);
    for (idx_t i = seq_len; i <= n; ++i)
    {
        if (i == 0)
        {
            seq_vals[0] = 1;
        }
        else if (i == 1)
        {
            seq_vals[1] = 0;
        }
        else
        {
            idx_t m = i - 1;                /* for binom(m,j) */
            seq_t sum = 0;
            seq_t binom = (seq_t)m;          /* binom(m,1) */
            for (idx_t j = 1; j <= m; ++j)
            {
                seq_t term = binom * pow_vals[j];
                term = term * seq_vals[i - 1 - j];
                sum = sum + term;
                if (j < m)
                {
                    /* binom(m,j+1) = binom * (m-j) / (j+1) */
                    seq_t numerator = binom * (m - j);
                    binom = numerator / (j + 1);
                }
            }
            seq_vals[i] = sum;
        }
    }
    seq_len = n + 1;
}

/* Required public function. */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return seq_vals[n];
}
