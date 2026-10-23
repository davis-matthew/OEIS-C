/*
 * A042885  Denominators of continued fraction convergents to sqrt(974).
 * https://oeis.org/A042885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 22, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   
   Continuation of the convergents to sqrt(974)
   a(0) = 1, a(1) = 4, a(2) = 5, …
   For n ≥ 2:
       a(n) = c[(n‑1) mod L] * a(n‑1) + a(n‑2)
   where c[0..L‑1] are the partial quotients of the continued
   fraction of sqrt(974) after the initial 31.
   The period length L turns out to be 32.
*/

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

static seq_t *denom = NULL;          /* computed denominators */
static size_t denom_len = 0;         /* number of stored denominators */
static size_t denom_cap = 0;         /* allocated capacity */

static uint32_t *cf = NULL;          /* periodic partial quotients */
static size_t cf_len = 0;            /* period length */
static size_t cf_cap = 0;            /* allocated capacity */

/* initialise the periodic partial quotients of sqrt(974) */
static void init_cf(void)
{
    if (cf_len != 0) return;        /* already initialised */

    const idx_t D = 974;
    const idx_t a0 = (idx_t)floorl(sqrtl((long double)D));

    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;

    while (1) {
        /* produce next partial quotient a1, a2, … */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        /* store a */
        if (cf_cap <= cf_len) {
            size_t new_cap = cf_cap ? cf_cap * 2 : 1;
            cf = (uint32_t *)realloc(cf, new_cap * sizeof(uint32_t));
            cf_cap = new_cap;
        }
        cf[cf_len++] = (uint32_t)a;

        if (a == 2 * a0) break;        /* period ends when a = 2*a0 */
    }
}

/* ensure that the denominator array has capacity and values up to n */
static void ensure_denoms(idx_t n)
{
    /* initialise denominators if needed */
    if (denom_len == 0) {
        denom_cap = 1;
        denom = (seq_t *)malloc(denom_cap * sizeof(seq_t));
        denom[0] = (seq_t)1;          /* a(0) = 1 */
        denom_len = 1;
    }

    /* enlarge array if necessary */
    if (n + 1 > denom_cap) {
        size_t new_cap = denom_cap ? denom_cap * 2 : 1;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        denom = (seq_t *)realloc(denom, new_cap * sizeof(seq_t));
        denom_cap = new_cap;
    }

    /* compute missing values */
    for (size_t i = denom_len; i <= (size_t)n; ++i) {
        uint32_t ak = cf[(i - 1) % cf_len];   /* partial quotient a_i */
        if (i == 1) {
            denom[i] = (seq_t)ak * denom[0];
        } else {
            denom[i] = (seq_t)ak * denom[i - 1] + denom[i - 2];
        }
    }
    denom_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    init_cf();
    ensure_denoms(n);
    return denom[(size_t)n];
}
