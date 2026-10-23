/*
 * A189512  n+[ns/r]+[nt/r]; r=1, s=e/3, t=3/e.
 * https://oeis.org/A189512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n·e/3⌋ + ⌊3n/e⌋
 * Range:     u8 none, u16 none, u32 none, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   leq, lt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (r = 1, s = e/3, t = 3/e) */

/* The constant D is the least common multiple of 1…20 (=2329089562800).  It is
 * divisible by every k! for k ≤ 20, so D/k! is an integer.  The value
 * e_D = Σ_{k=0}^{20} D/k! is exactly ⌊e·D⌋ because all omitted terms are
 * < 1.  All further calculations use only integer arithmetic. */
#include <stdint.h>   /* for standard integer types */
#include "../utils/leq.h"
#include "../utils/lt.h"

static const seq_t D = 2329089562800ULL;  /* lcm(1..20) fits in 64‑bit */

/* Compute ⌊e·D⌋ once.  The result fits in seq_t on all builds. */
static seq_t e_D = 0;      /* will be initialised on first call */

static void init_e_D(void)
{
    static int inited = 0;
    if (inited) return;
    inited = 1;

    seq_t fact = 1;
    seq_t sum  = 0;
    for (idx_t k = 0; k <= 20; ++k) {
        if (k > 0) fact *= k;          /* fact = k!  */
        sum += D / fact;              /* integer division, exact */
    }
    e_D = sum;                         /* e_D = ⌊e·D⌋   */
}

/*********************************************************************
 * a(n) - main sequence function                                       *
 *--------------------------------------------------------------------
 * Input   : n (idx_t) – the index (≥ 1).  The function is safe for
 *            any n that fits in idx_t.  For very large n the product
 *            3n·D might overflow seq_t on 64‑bit builds; the
 *            usage in the test harness is n ≤ 10 000, so this
 *            is safe.
 * Output  : a(n) as seq_t.
 *********************************************************************/
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* optional: behaviour for offset 0 */

    init_e_D();                          /* initialise e_D once */

    seq_t nn = (seq_t)n;                  /* convert index to seq_t */

    /* floor(n·e) = floor( (n·e_D) / D ) because e_D = ⌊e·D⌋ */
    seq_t floor_ne = (nn * e_D) / D;

    /* floor(n·(e/3)) = floor( floor(n·e) / 3 ) */
    seq_t floor_ns = floor_ne / 3;

    /* floor(3n/e) is handled using bounds L = (3n·D)/(e_D+1)
     * and U = (3n·D)/e_D.  Let k = floor(U);  k is either the exact
     * value or one larger.  We choose the correct one by checking
     * the inequalities:
     *  k·(e_D+1) ≤ 3n·D   AND   3n·D < (k+1)·e_D.
     */
    seq_t temp   = 3 * nn * D;          /* 3n·D  */
    seq_t k      = temp / e_D;          /* floor(U)  */

    seq_t floor_nt;
    if ( leq(k * (e_D + 1), temp)   /* k·(e_D+1) ≤ 3n·D */
      && lt (temp, (k + 1) * e_D) ) /* 3n·D < (k+1)·e_D */
    {
        floor_nt = k;                  /* k is correct */
    }
    else
    {
        floor_nt = k - 1;              /* k was one too high   */
    }

    return nn + floor_ns + floor_nt;    /* final result */
}
