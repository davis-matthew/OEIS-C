/*
 * A232180  First bisection of harmonic numbers (numerators).
 * https://oeis.org/A232180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = Numerator of the harmonic number H(2n-1), where H(m) = 1
 *            + 1/2 + ... + 1/m, expressed in lowest terms
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: accumulate H(m) for all m up to 2n-1,
   reducing each intermediate fraction by gcd, and store
   the numerator only when m is odd. */
#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *numerators = NULL;   /* array of numerators for odd m */
static size_t numerators_cap = 0;  /* allocated capacity of numerators array */
static size_t n_terms = 0;         /* number of numerators stored (== n-1 for current max n) */
static seq_t cur_num = 0;          /* numerator of H(current_m) */
static seq_t cur_den = 1;          /* denominator of H(current_m) */
static idx_t current_m = 1;        /* current m for which H(cur_m) has been computed */

/* ensure the numerators array has space for at least (need) elements */
static void ensure_numerators(size_t need)
{
    if (need <= numerators_cap) return;
    size_t new_cap = numerators_cap ? numerators_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    numerators = (seq_t*)realloc(numerators, new_cap * sizeof(seq_t));
    numerators_cap = new_cap;
}

/* one-time initialization for n = 1 (H_1 = 1/1) */
static void init(void)
{
    if (n_terms != 0) return;   /* already initialized */
    current_m = 1;
    cur_num   = 1;
    cur_den   = 1;
    ensure_numerators(1);
    numerators[0] = 1;
    n_terms = 1;
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1; undefined for n < 1 */
    init();

    if ((idx_t)n <= n_terms) return numerators[n-1];

    idx_t target_m = 2 * n - 1;   /* last m we need to compute up to */

    for (idx_t m = current_m + 1; m <= target_m; ++m) {
        seq_t tmp_num = cur_num * (seq_t)m + cur_den;
        seq_t tmp_den = cur_den * (seq_t)m;
        seq_t g = gcd(tmp_num, tmp_den);
        tmp_num /= g;
        tmp_den /= g;
        cur_num = tmp_num;
        cur_den = tmp_den;
        if (m & 1) {               /* m is odd -> store numerator */
            ensure_numerators(n_terms + 1);
            numerators[n_terms] = cur_num;
            ++n_terms;
        }
    }
    current_m = target_m;
    return numerators[n-1];
}
