/*
 * A115945  Numbers that cannot be written as a sum of distinct factorials.
 * https://oeis.org/A115945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n·(n+1)/2
 * Range:     u8 n <= 224, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gen_representable
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115945: Numbers that cannot be written as a sum of distinct factorials */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gen_representable.h"

/* limits */
#define REP_LIMIT 2000000ULL   /* enough for the first 10000 terms */

/* static storage */
static bool *rep = NULL;
static bool rep_initialized = false;

/* one‑time initialization */
static void init_rep(void)
{
    if (rep_initialized) return;
    rep = (bool*)calloc(REP_LIMIT + 1, sizeof(bool));
    if (!rep) exit(1);  /* allocation failure */

    /* compute factorials up to REP_LIMIT */
    unsigned long long fac[20];
    fac[0] = 1ULL;
    for (int i = 1; i < 20; ++i)
        fac[i] = fac[i - 1] * (unsigned long long)i;

    /* collect factorials 1! ... that are <= REP_LIMIT */
    unsigned long long farr[20];
    int k = 0;
    for (int i = 1; fac[i] <= REP_LIMIT; ++i)
        farr[k++] = fac[i];

    /* generate all distinct‑factorial sums */
    gen_representable(0, 0ULL, farr, k, REP_LIMIT, rep);

    rep_initialized = true;
}

/* sequence function */
seq_t a(idx_t n)
{
    init_rep();

    size_t cnt = 0;
    for (size_t i = 1; i <= REP_LIMIT; ++i) {
        if (!rep[i]) {
            cnt++;
            if (cnt == n)
                return (seq_t)i;
        }
    }
    /* if the requested term is beyond the pre‑computed range, */
/* this should not happen for the intended test limits. */
    return (seq_t)0;
}
