/*
 * A025545  a(n) = (1/4 + 1/6 + ... + 1/c(n))*LCM{4, 6, ..., c(n)}, where c(n)
 *          = n-th composite number.
 * https://oeis.org/A025545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = LCM{4,6,...,c(n)} * (1/4 + 1/6 + ... + 1/c(n)) where c(n)
 *            is the n-th composite number. Since LCM divides each
 *            denominator, a(n) = Σ_{j=1..n} L(n)/c(j)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   gcd, grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: this implementation uses the utilities gcd, is_prime,
   and grow_seq provided by the build environment. */

#include <stdbool.h>
#include "../utils/gcd.h"        /* seq_t gcd(seq_t, seq_t) */
#include "../utils/is_prime.h"   /* bool is_prime(idx_t) */
#include "../utils/grow_seq.h"   /* void grow_seq(seq_t**, size_t*, size_t) */

static idx_t comp_next = 4;                    /* candidate for next composite */
static idx_t comp_cnt  = 0;                    /* number of composites stored */
static seq_t *comp_list = NULL;                /* array of composites */
static size_t comp_cap = 0;                    /* capacity of comp_list */

static void ensure_composites(idx_t n)
{
    while (comp_cnt < n) {
        /* find next composite */
        while (is_prime(comp_next)) comp_next++;
        if (comp_cnt >= comp_cap)
            grow_seq(&comp_list, &comp_cap, comp_cnt + 1);
        comp_list[comp_cnt++] = (seq_t)comp_next;
        comp_next++;
    }
}

static idx_t max_computed = 0;               /* highest n we've computed */
static seq_t *L_arr   = NULL;                 /* L(n) values */
static seq_t *a_arr   = NULL;                 /* a(n) values */
static size_t L_cap   = 0;
static size_t a_cap   = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for 0, return 0 */

    ensure_composites(n);

    if (max_computed == 0) {
        /* initialise for n = 1 */
        if (L_cap < 2) grow_seq(&L_arr, &L_cap, 2);
        if (a_cap < 2) grow_seq(&a_arr, &a_cap, 2);
        L_arr[1] = (seq_t)comp_list[0];     /* first composite is 4 */
        a_arr[1] = 1;                       /* 4 * (1/4) = 1 */
        max_computed = 1;
    }

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t c = comp_list[i - 1];          /* composite c(i) */
        seq_t L_prev = L_arr[i - 1];
        seq_t g = gcd(L_prev, c);
        seq_t factor = c / g;
        seq_t L_new = L_prev * factor;
        seq_t a_new = factor * a_arr[i - 1] + L_prev / g;

        if (i > L_cap) grow_seq(&L_arr, &L_cap, i + 1);
        if (i > a_cap) grow_seq(&a_arr, &a_cap, i + 1);
        L_arr[i] = L_new;
        a_arr[i] = a_new;
    }

    max_computed = n;
    return a_arr[n];
}
