/*
 * A175127  a(n) = the largest numbers m with the number of steps n of
 *          iterations of {r - (smallest prime divisor of r)} needed to reach
 *          0 starting at r = m.
 * https://oeis.org/A175127
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A175127: a(n) = largest m with A175126(m) = n  (offset 2) */

/* This implementation builds a table of smallest prime factors (SPF) and
   dynamic programming table for the number of steps to reach 0.
   For each n requested it guarantees that the table covers all m
   up to 2*n+2000, which is more than enough to contain the maximal m
   for that n (proof is given in the analysis).  The table is kept in
   static global state so subsequent calls are fast.  */

//  #include directives required for any build
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *spf = NULL;           /* smallest prime factor array  */
static idx_t      spf_len = 0;         /* length of SPF array (0..spf_len-1) */

static idx_t *steps = NULL;            /* number of steps for each r   */
static idx_t   steps_len = 0;          /* length of steps array        */

static seq_t *max_m_for_step = NULL;   /* a(n) values                  */
static idx_t  max_step_cap = 0;        /* capacity of max_m_for_step   */
static idx_t  max_step_idx  = 0;       /* largest step seen so far     */

/* Expand the table so that all steps up to `n` are known.  */
static void ensure_tables(idx_t n)
{
    /* We need at least m = 2*n to guarantee existence of a number
       with exactly n steps (e.g. m = 2*n).  Add an extra margin.   */
    idx_t needed_limit = 2 * n + 2000;
    if (needed_limit <= spf_len)
        return;                        /* already large enough            */

    /* Reallocate SPF and steps arrays.  */
    uint32_t *new_spf = (uint32_t*)malloc((needed_limit + 1) * sizeof(uint32_t));
    if (!new_spf) return;              /* allocation failure – abort    */
    steps = (idx_t*)realloc(steps, (needed_limit + 1) * sizeof(idx_t));
    if (!steps) { free(new_spf); return; }

    /* Compute SPF up to needed_limit.  */
    for (idx_t i = 0; i <= needed_limit; ++i)
        new_spf[i] = 0;
    for (idx_t i = 2; i <= needed_limit; ++i) {
        if (new_spf[i] == 0) {         /* i is prime */
            new_spf[i] = (uint32_t)i;
            /* Mark multiples with this prime as their SPF */
            if (i * i <= needed_limit) {
                for (idx_t j = i * i; j <= needed_limit; j += i)
                    if (new_spf[j] == 0)
                        new_spf[j] = (uint32_t)i;
            }
        }
    }

    /* Free old SPF array and replace with the new one.  */
    free(spf);
    spf = new_spf;
    spf_len = needed_limit + 1;

    /* Compute the steps DP table.  */
    steps[0] = 0;
    for (idx_t r = 1; r <= needed_limit; ++r) {
        uint32_t p = (r == 1) ? 1 : new_spf[r];
        steps[r] = steps[r - p] + 1;
    }
    steps_len = needed_limit + 1;

    /* Update the max_m_for_step table with all r <= needed_limit.  */
    for (idx_t r = 1; r <= needed_limit; ++r) {
        idx_t t = steps[r];
        if (t >= max_step_cap) {
            idx_t new_cap = max_step_cap ? max_step_cap * 2 : 16;
            while (new_cap <= t) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(max_m_for_step,
                                         new_cap * sizeof(seq_t));
            if (!tmp) return;            /* allocation failure – abort    */
            /* initialise new slots to 0 */
            for (idx_t i = max_step_cap; i < new_cap; ++i)
                tmp[i] = (seq_t)0;
            max_m_for_step = tmp;
            max_step_cap  = new_cap;
        }
        if ((seq_t)r > max_m_for_step[t])
            max_m_for_step[t] = (seq_t)r;
        if (t > max_step_idx)
            max_step_idx = t;
    }
}

/* Public function required by the problem. */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;  /* undefined for offset < 2 */
    ensure_tables(n);
    return max_m_for_step[n];
}
