/*
 * A080727  a(0) = 1; for n>0, a(n) is taken to be the smallest positive
 *          integer greater than a(n-1) which is consistent with the condition
 *          "n is a member of the sequence if and only if a(n) == 2 mod 3".
 * https://oeis.org/A080727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A080727 :  a(0) = 1;  a(a(n)) = 3*n + 2   (n ≥ 0)
    (implemented by maintaining the functional constraints and
     selecting the smallest unused integer greater than a(n‑1)). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* --------  dynamic arrays for the sequence  -------- */
static seq_t *store     = NULL;   /* a[i]   */
static size_t storeSize = 0;
static idx_t  processed = 0;      /* largest n whose a(n) is known */

/* --------  pre‑assigned future values (constraints) -------- */
static seq_t *needIdx   = NULL;   /* needIdx[k] = forced a(k) (0 if unknown) */
static size_t needSize  = 0;

/* --------  set of all values that have already appeared in the sequence -------- */
static seq_t *usedVals  = NULL;   /* sorted array of used values */
static size_t usedSize  = 0;
static size_t usedCap   = 0;

/* --------  helpers for dynamic arrays -------- */
static void ensure_store_size(size_t minSize) {
    if (storeSize >= minSize) return;
    size_t newSize = storeSize ? storeSize << 1 : 8;
    while (newSize < minSize) newSize <<= 1;
    seq_t *tmp = (seq_t *)realloc(store, newSize * sizeof(seq_t));
    if (!tmp) abort();
    store = tmp;
    storeSize = newSize;
}

static void ensure_need_size(size_t minSize) {
    if (needSize >= minSize) return;
    size_t newSize = needSize ? needSize << 1 : 8;
    while (newSize < minSize) newSize <<= 1;
    seq_t *tmp = (seq_t *)realloc(needIdx, newSize * sizeof(seq_t));
    if (!tmp) abort();
    /* zero the newly allocated part */
    memset(tmp + needSize, 0, (newSize - needSize) * sizeof(seq_t));
    needIdx = tmp;
    needSize = newSize;
}

static void ensure_used_size(size_t minSize) {
    if (usedCap >= minSize) return;
    size_t newCap = usedCap ? usedCap << 1 : 8;
    if (newCap < minSize) newCap = minSize;
    seq_t *tmp = (seq_t *)realloc(usedVals, newCap * sizeof(seq_t));
    if (!tmp) abort();
    usedVals = tmp;
    usedCap = newCap;
}

static bool used_is_used(seq_t val) {
    size_t lo = 0, hi = usedSize;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (usedVals[mid] < val) lo = mid + 1;
        else if (usedVals[mid] > val) hi = mid;
        else return true;
    }
    return false;
}

/* insert a value into the sorted usedVals array (if not already present) */
static void add_used_val(seq_t val) {
    /* binary search for insertion point */
    size_t lo = 0, hi = usedSize;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (usedVals[mid] < val) lo = mid + 1;
        else hi = mid;
    }
    if (lo < usedSize && usedVals[lo] == val) return;   /* already present */
    ensure_used_size(usedSize + 1);
    memmove(usedVals + lo + 1, usedVals + lo, (usedSize - lo) * sizeof(seq_t));
    usedVals[lo] = val;
    usedSize++;
}

/* --------  the sequence function -------- */
seq_t a(idx_t n) {
    /* one‑time initialisation */
    static bool initialized = false;
    if (!initialized) {
        ensure_store_size(8);
        ensure_need_size(8);
        ensure_used_size(8);

        store[0] = (seq_t)1;        /* a(0) = 1 */
        add_used_val((seq_t)1);

        /* From the functional equation: a(a(0)) = 3*0 + 2 = 2 */
        idx_t idx1 = 1;
        if (idx1 >= needSize) ensure_need_size(idx1 + 1);
        needIdx[idx1] = (seq_t)2;
        add_used_val((seq_t)2);

        processed = 0;
        initialized = true;
    }

    /* if already computed, just return */
    if (n <= processed) return store[n];

    /* compute missing values up to n */
    for (idx_t i = processed + 1; i <= n; ++i) {
        seq_t val;

        /* 1) honour any pre‑assigned constraint for i */
        if (i < needSize && needIdx[i] != (seq_t)0) {
            val = needIdx[i];
            needIdx[i] = 0; /* consumed */
        } else {
            /* 2) choose the smallest integer > a(i‑1) that is unused */
            val = store[i - 1] + (seq_t)1;
            while (used_is_used(val)) val = val + (seq_t)1;
        }

        /* store the value */
        if (i >= storeSize) ensure_store_size((size_t)i + 1);
        store[i] = val;
        add_used_val(val);

        /* 3) forward constraint coming from this value */
        idx_t valIdx = (idx_t)val;           /* assumes val fits in idx_t */
        if (valIdx >= needSize) ensure_need_size((size_t)valIdx + 1);
        needIdx[valIdx] = (seq_t)((seq_t)3 * (seq_t)i + (seq_t)2);

        processed = i;
    }

    return store[n];
}
