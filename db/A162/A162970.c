/*
 * A162970  Number of 2-cycles in all involutions of {1,2,...,n}.
 * https://oeis.org/A162970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 30, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A162970: Number of 2‑cycles in all involutions of {1,2,…,n}.
   Formula:  a(n) = n*(n-1)/2 * I(n-2)   (n ≥ 2)  where I(k) is the
   number of involutions of {1,…,k} (OEIS A000085).
   Initial values:  a(1)=0,  a(2)=1.
   I satisfies I(0)=1, I(1)=1 and I(k)=I(k-1)+(k-1)*I(k-2).
*/

#include <stdint.h>

static seq_t I_cache[20001];   /* involutions count */
static seq_t a_cache[20001];   /* desired sequence values */
static idx_t I_max = 1;        /* largest k computed in I_cache  */
static idx_t a_max = 2;        /* largest n computed in a_cache  */
static int  init_done = 0;     /* flag to ensure one-time initialization */

static void ensure_initialized(void)
{
    if (init_done) return;
    /* initialise base cases */
    I_cache[0] = (seq_t)1;
    I_cache[1] = (seq_t)1;
    a_cache[1] = (seq_t)0;
    a_cache[2] = (seq_t)1;
    init_done = 1;
}

static void compute_I_up_to(idx_t upto)
{
    if (upto <= I_max) return;
    for (idx_t i = I_max + 1; i <= upto; ++i)
    {
        I_cache[i] = I_cache[i-1] + (seq_t)(i-1) * I_cache[i-2];
    }
    I_max = upto;
}

static void compute_a_up_to(idx_t upto)
{
    if (upto <= a_max) return;
    /* ensure enough I values are available */
    compute_I_up_to(upto - 2);

    for (idx_t i = a_max + 1; i <= upto; ++i)
    {
        seq_t term = ((seq_t)i * (seq_t)(i-1) / (seq_t)2) * I_cache[i-2];
        a_cache[i] = term;
    }
    a_max = upto;
}

seq_t a(idx_t n)
{
    ensure_initialized();
    if (n == 0) return (seq_t)0;   /* undefined by OEIS but safe */
    if (n <= a_max) return a_cache[n];
    compute_a_up_to(n);
    return a_cache[n];
}
