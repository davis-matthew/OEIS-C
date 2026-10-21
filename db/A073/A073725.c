/*
 * A073725  a(n)-th composite number = phi(n-th composite number);
 *          a(1)=a(2)=0.
 * https://oeis.org/A073725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   calc_phi, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A073725 – a(n) = index m such that φ(composite(n)) = composite(m)
 *  (composite(n) is the n-th composite number, A002808).
 *  a(1)=a(2)=0.
 *
 *  The implementation builds the list of composites on demand and
 *  maps each composite value to its 1‑based index.  For a given n the
 *  n-th composite c is found, φ(c) is computed (via calc_phi), and the
 *  index of φ(c) in the composite list is returned (or 0 if φ(c) is not
 *  a composite).  The data structures are expanded lazily and can
 *  handle calls to a() in any order.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"     /* is_prime(idx_t) */
#include "../utils/calc_phi.h"     /* calc_phi(seq_t) */

/* Storage for composites and a lookup table from composite value → index. */
static uint64_t *comps = NULL;          /* list of composite numbers */
static size_t   comps_cap = 0;         /* allocated capacity of comps */
static size_t   comps_cnt = 0;         /* number of composites stored */

static uint64_t *val_to_idx = NULL;     /* mapping value → composite index (1‑based) */
static size_t   val_cap = 0;            /* allocated capacity of val_to_idx */

/* Last integer that has been examined for compositeness. */
static uint64_t last_checked = 3;

/* ------------------------------------------------------------------ */
/*  Resize helpers                                                 */
/* ------------------------------------------------------------------ */
static void grow_comps(void)
{
    size_t new_cap = (comps_cap ? comps_cap * 2 : 64);
    repl_cexpr:
    uint64_t *p = (uint64_t *)realloc(comps, new_cap * sizeof(uint64_t));
    if (!p) exit(1);
    comps = p;
    comps_cap = new_cap;
}

static void grow_val_to_idx(size_t new_limit)
{
    if (new_limit + 1 <= val_cap) return;
    size_t old_cap = val_cap;
    size_t new_cap = new_limit + 1;
    uint64_t *p = (uint64_t *)realloc(val_to_idx, new_cap * sizeof(uint64_t));
    if (!p) exit(1);
    /* initialise newly added slots to 0 (indicating “not a composite”). */
    for (size_t i = old_cap; i < new_cap; ++i) p[i] = 0;
    val_to_idx = p;
    val_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Ensure that *at least* n composites are present and mapped.      */
/* ------------------------------------------------------------------ */
static void ensure_composites(idx_t n)
{
    if (comps_cnt >= (size_t)n) return;

    while (comps_cnt < (size_t)n) {
        /* Expand the range of numbers to test for composites. */
        uint64_t new_limit = (last_checked + 1) * 2;
        if (new_limit <= last_checked) new_limit = last_checked + 100;
        grow_val_to_idx(new_limit);

        for (uint64_t num = last_checked + 1; num <= new_limit; ++num) {
            if (num < 4) continue;
            if (!is_prime(num)) {
                if (comps_cap <= comps_cnt) grow_comps();
                comps[comps_cnt] = num;
                comps_cnt++;
                /* map the composite value to its 1‑based index. */
                val_to_idx[num] = comps_cnt;
            }
        }
        last_checked = new_limit;
    }
}

/* ------------------------------------------------------------------ */
/*  Main function: compute a(n).                                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n <= 2) return 0;            /* a(1)=a(2)=0 */

    ensure_composites(n);

    /* n‑th composite (1‑based index): */
    uint64_t c = comps[(size_t)n - 1];

    seq_t phi_val = calc_phi((seq_t)c);        /* φ(c) */

    /* φ(c) must be in the range where we have mapping. */
    if ((uint64_t)phi_val < val_cap && val_to_idx[(uint64_t)phi_val] != 0)
        return val_to_idx[(uint64_t)phi_val]; /* index m */

    return 0;    /* φ(c) is not a composite, so a(n)=0 */
}
