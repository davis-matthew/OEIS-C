/*
 * A048978  Light numbers, those whose mean sum of divisors <= mean running
 *          sum of divisors of all previous n.
 * https://oeis.org/A048978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 957, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Light numbers: a(n) = the nth integer n such that
   σ(n)/τ(n) ≤ (∑_{k=1}^{n-1} σ(k)) / (∑_{k=1}^{n-1} τ(k)).
   Here σ is the sum of divisors, τ is the number of divisors.
   The sequence starts with 360,630,720,...
   The algorithm keeps running sums of σ and τ and tests each candidate.
*/
#include <stdlib.h>
#include <stdbool.h>

/* ---- static state ---- */
static seq_t *sigma = NULL;           /* sigma[0] unused */
static seq_t *tau   = NULL;           /* tau[0]   unused */
static idx_t  sigma_size = 0;         /* current array size (>= index used +1) */

static seq_t rs = 0;                  /* running sum of sigma */
static seq_t rd = 0;                  /* running sum of tau */
static idx_t last_checked = 0;        /* last natural number processed */

static seq_t *lights = NULL;          /* array of light numbers found so far */
static size_t lights_cap = 0;          /* capacity of lights array */
static size_t light_count = 0;         /* number of light numbers stored */

/* ---- utility to extend sigma and tau arrays up to 'cap' ---- */
static void ensure_sigma_tau_up_to(idx_t cap)
{
    if (cap < sigma_size) return;

    idx_t new_cap = sigma_size ? sigma_size : 1;
    while (new_cap <= cap) new_cap <<= 1;

    seq_t *new_sigma = (seq_t *)realloc(sigma, new_cap * sizeof(seq_t));
    seq_t *new_tau   = (seq_t *)realloc(tau,   new_cap * sizeof(seq_t));

    if (!new_sigma || !new_tau) {
        exit(1);
    }

    /* initialise new elements to zero */
    for (idx_t i = sigma_size; i < new_cap; ++i) {
        new_sigma[i] = 0;
        new_tau[i]   = 0;
    }

    /* add divisor contributions for the new range [sigma_size, new_cap) */
    for (idx_t i = 1; i < new_cap; ++i) {
        /* first multiple of i that is ≥ sigma_size */
        idx_t first = ((sigma_size + i - 1) / i) * i;
        if (first < i) first = i;                   /* ensure we start from i itself   */
        if (first < sigma_size) first = sigma_size; /* ensure we start from new part */

        for (idx_t j = first; j < new_cap; j += i) {
            new_sigma[j] += (seq_t)i;
            new_tau[j]   += 1;
        }
    }

    sigma = new_sigma;
    tau   = new_tau;
    sigma_size = new_cap;
}

/* ---- expand lights array when needed ---- */
static void ensure_lights_capacity(void)
{
    if (light_count < lights_cap) return;
    size_t new_cap = lights_cap ? lights_cap << 1 : 16;
    seq_t *new_arr = (seq_t *)realloc(lights, new_cap * sizeof(seq_t));
    if (!new_arr) exit(1);
    lights = new_arr;
    lights_cap = new_cap;
}

/* ---- requested function ---- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, return 0 */

    /* generate light numbers until we have at least n of them */
    while (light_count < (size_t)n) {
        idx_t cand = last_checked + 1;
        ensure_sigma_tau_up_to(cand);

        seq_t s_val = sigma[cand];
        seq_t d_val = tau[cand];

        /* candidate is light only if there is a previous mean to compare */
        if (rd > 0) {
            /* s_val / d_val <= rs / rd   ⇔   s_val * rd <= rs * d_val   */
            if (s_val * rd <= rs * d_val) {
                ensure_lights_capacity();
                lights[light_count++] = cand;
            }
        }

        /* update running sums for all numbers */
        rs += s_val;
        rd += d_val;
        last_checked = cand;
    }

    return lights[n - 1];
}
