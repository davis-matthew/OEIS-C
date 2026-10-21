/*
 * A086482  Beginning with 1, the smallest number not included earlier such
 *          that the n-th partial product is an n-th power; or the geometric
 *          mean of the first n terms is an integer.
 * https://oeis.org/A086482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^{e(n)} where e(1)=0 and for n≥2, e(n) is the smallest
 *            unused non‑negative integer k such that (∑_{i=1}^{n} e(i)) ≡ 0
 *            (mod n). The sequence a(n) consists solely of powers of 2
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* This function is built‑in: idx_t is an unsigned integer type
   (typically uint64_t) and seq_t is an unsigned integer type of
   8, 16, 32, 64 or 128 bits, or an arbitrary‑precision integer
   class.  No header is required for these types, they are
   supplied by the build. */

static idx_t *exps = NULL;          /* exponent of a(i)   */
static idx_t exps_cap = 0;          /* capacity of exps   */

static bool *used = NULL;           /* used[exponent] = true if exponent already occurred */
static idx_t used_cap = 0;          /* capacity of used   */

static idx_t max_computed = 0;      /* highest index computed so far */
static idx_t sum_exps   = 0;        /* ∑_{i=1}^{max_computed} e(i) */

/* Ensure `used` can index exponent k */
static void ensure_used(idx_t k)
{
    if (k < used_cap) return;
    idx_t new_cap = used_cap ? (used_cap << 1) : 64;
    while (new_cap <= k) new_cap <<= 1;
    bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!tmp) exit(EXIT_FAILURE);
    for (idx_t i = used_cap; i < new_cap; ++i) tmp[i] = false;
    used = tmp;
    used_cap = new_cap;
}

/* Ensure `exps` can store up to index n (1‑based) */
static void ensure_exps(idx_t n)
{
    if (n < exps_cap) return;
    idx_t new_cap = exps_cap ? (exps_cap << 1) : 64;
    while (new_cap <= n) new_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(exps, new_cap * sizeof(idx_t));
    if (!tmp) exit(EXIT_FAILURE);
    exps = tmp;
    exps_cap = new_cap;
}

/* Extend the precomputation to at least index n */
static void cache_up_to(idx_t n)
{
    if (n == 0 || n <= max_computed) return;

    /* initialise a(1)=1 (exponent 0) if necessary */
    if (max_computed == 0) {
        ensure_exps(1);
        exps[1] = 0;
        ensure_used(0);
        used[0] = true;
        sum_exps = 0;
        max_computed = 1;
    }

    ensure_exps(n);

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* required residue: k ≡ -sum_exps (mod i) */
        idx_t rem = sum_exps % i;
        idx_t k = (i - rem) % i;          /* 0 ≤ k < i */

        /* skip already used exponents in this progression */
        while (true) {
            ensure_used(k);
            if (!used[k]) break;
            k += i;
        }

        used[k]   = true;
        exps[i]   = k;
        sum_exps += k;
    }

    max_computed = n;
}

/* Public interface */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety; sequence is defined for n≥1 */
    cache_up_to(n);
    idx_t e = exps[n];

    seq_t val = (seq_t)1;
    for (idx_t i = 0; i < e; ++i) {
        val *= (seq_t)2;
    }
    return val;
}
