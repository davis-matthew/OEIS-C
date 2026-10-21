/*
 * A074987  a(n) is the least m not equal to n such that phi(m) = phi(n).
 * https://oeis.org/A074987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): smallest m ≠ n with φ(m) = φ(n) */

#include "../utils/phi.h"
#include <stdbool.h>
#include <stdlib.h>

/* pair holding the two smallest numbers that give the same phi value */
typedef struct {
    seq_t first;   /* smallest m */
    seq_t second;  /* second smallest m (0 if none yet) */
} pair_t;

/* pre‑compute φ for all m ≤ LIMIT and build the map */
#define LIMIT 200000          /* sufficient for the required test range */

static pair_t *map = NULL;
static bool initialized = false;

static void init(void)
{
    if (initialized)
        return;

    /* allocate the map (size LIMIT+1 to index by φ value) */
    map = (pair_t *)calloc((size_t)LIMIT + 1, sizeof(pair_t));
    if (!map) {
        abort();   /* should never happen for the chosen LIMIT */
    }

    for (idx_t m = 1; m <= LIMIT; ++m) {
        seq_t phi_m = phi(m);
        idx_t key = (idx_t)phi_m;        /* φ(m) ≤ LIMIT‑1 for m ≤ LIMIT */

        pair_t *p = &map[key];

        if (p->first == 0 || m < (idx_t)p->first) {
            p->second = p->first;
            p->first  = (seq_t)m;
        } else if (p->second == 0 || m < (idx_t)p->second) {
            p->second = (seq_t)m;
        }
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    init();

    if (n <= LIMIT) {
        seq_t phi_n = phi(n);
        idx_t key   = (idx_t)phi_n;

        pair_t *p = &map[key];
        if (p->first == n) {                /* n is the smallest for this φ */
            if (p->second != 0)
                return p->second;
            /* no second entry yet – fall back to exhaustive search */
        } else {
            /* an even smaller m exists; that is the answer */
            return p->first;
        }
    }

    /* For n beyond the pre‑computed range: brute‑force search */
    seq_t phi_n = phi(n);
    for (idx_t m = 1; ; ++m) {
        if (m == n)
            continue;
        if (phi(m) == phi_n)
            return (seq_t)m;
    }
}
