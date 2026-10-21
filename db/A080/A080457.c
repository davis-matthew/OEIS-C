/*
 * A080457  a(1)=3; for n>1, a(n)=a(n-1) if n is already in the sequence,
 *          a(n)=a(n-1)+4 otherwise.
 * https://oeis.org/A080457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=3; for n>1, a(n) = a(n-1) if n already appears as a term,
 *            else a(n) = a(n-1) + 4. The function memoises values and keeps a
 *            bitset of all values that have appeared, so membership tests are
 *            O(1)
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

extern seq_t a(idx_t n);     /* Declaration used by the build system */

/* ----------  static state  ---------- */
static seq_t *vals          = NULL;   /* 1‑indexed array of terms */
static size_t cap_vals      = 0;      /* capacity of vals (number of entries) */
static idx_t  used_vals      = 0;      /* largest n for which vals[n] is valid */

static uint8_t *present      = NULL;   /* presence table: present[x] == 1 if value x has occurred */
static size_t  cap_present    = 0;      /* number of bytes allocated in present */

static int initialized = 0;

/* Ensure present[] can store indices up to v (inclusive). */
static void ensure_present_capacity(idx_t v)
{
    if ((size_t)v + 1 > cap_present) {
        size_t new_cap = cap_present ? cap_present * 2 : 16;
        while ((size_t)v + 1 > new_cap)
            new_cap <<= 1;
        uint8_t *tmp = (uint8_t*)realloc(present, new_cap);
        if (!tmp) exit(1);
        /* Zero the newly allocated portion */
        memset(tmp + cap_present, 0, new_cap - cap_present);
        present = tmp;
        cap_present = new_cap;
    }
}

/* Ensure vals[] can index up to n (inclusive). */
static void ensure_vals_capacity(idx_t n)
{
    if ((size_t)n + 1 > cap_vals) {
        size_t new_cap = cap_vals ? cap_vals * 2 : 16;
        while ((size_t)n + 1 > new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(vals, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        vals = tmp;
        cap_vals = new_cap;
    }
}

/* ----------  public function  ---------- */
seq_t a(idx_t n)
{
    if (n < 1)                     /* behaviour for n < 1 is irrelevant */
        return 0;

    if (!initialized) {
        /* initialise values array to hold at least index 1 */
        ensure_vals_capacity(1);
        vals[1] = (seq_t)3;
        used_vals = 1;

        /* initialise presence table to hold value 3 */
        ensure_present_capacity((idx_t)3);
        present[3] = 1;

        initialized = 1;
    }

    if (n <= used_vals)            /* cached result */
        return vals[n];

    /* make sure the array can hold up to n */
    ensure_vals_capacity(n);

    /* compute missing terms up to n */
    for (idx_t i = used_vals + 1; i <= n; ++i) {
        seq_t prev = vals[i - 1];
        seq_t cur;

        /* check if i has already appeared as a term */
        if (i < (idx_t)cap_present && present[i])
            cur = prev;                /* keep the same value */
        else {
            cur = prev + (seq_t)4;     /* increase by 4 */
            /* mark this new value as present if it could be an index */
            idx_t val_idx = (idx_t)cur;
            ensure_present_capacity(val_idx);
            present[val_idx] = 1;
        }

        vals[i] = cur;
    }

    used_vals = n;
    return vals[n];
}
