/*
 * A214370  If a(n) has not yet been defined then set a(n) = least positive
 *          integer that has not yet occurred; also if a(n+1+a(n)) has not yet
 *          been defined then set a(n+1+a(n)) = a(n).
 * https://oeis.org/A214370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = least positive integer not yet used;
 * if a(n) is newly defined, set a(n+1+a(n)) = a(n) provided that
 * position is still undefined.  offset 1; a(1)=1, a(2)=2.
 * This implementation memoises the sequence with dynamically expanding
 * arrays and works for both C (seq_t is an integer type) and C++ 
 * (seq_t may be an arbitrary‑precision integer class). */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* static cache of terms (1‑based). */
static seq_t *A = NULL;       /* array of sequence terms */
static size_t capA = 0;       /* current capacity of A (index 0 unused) */
static idx_t max_n = 0;       /* highest index that has been processed */

/* bit array noting which integer values have already appeared */
static uint8_t *used = NULL;  /* used[v] == 1 if value v has appeared */
static size_t cap_used = 0;   /* capacity of used array (index 0 unused) */
static idx_t next_unused = 1; /* smallest positive integer not yet used */

/* ------------------------------------------------------------------ */
/* Helper to ensure A has room for index idx (1‑based). */
static void ensure_capA(idx_t idx)
{
    size_t needed = (size_t)idx + 1;          /* include index 0 */
    if (needed > capA) {
        size_t newcap = capA ? capA * 2 : 64;
        while (newcap < needed) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(A, (newcap + 1) * sizeof(seq_t));
        if (!tmp) abort();                    /* allocation failure */
        /* zero any newly added elements (treat as zero) */
        memset(tmp + capA, 0, (newcap + 1 - capA) * sizeof(seq_t));
        A = tmp;
        capA = newcap;
    }
}

/* Helper to ensure used[] has room for value v (1‑based). */
static void ensure_cap_used(idx_t v)
{
    size_t needed = (size_t)v + 1;
    if (needed > cap_used) {
        size_t newcap = cap_used ? cap_used * 2 : 64;
        while (newcap < needed) newcap <<= 1;
        uint8_t *tmp = (uint8_t *)realloc(used, newcap * sizeof(uint8_t));
        if (!tmp) abort();
        memset(tmp + cap_used, 0, (newcap - cap_used) * sizeof(uint8_t));
        used = tmp;
        cap_used = newcap;
    }
}

/* ------------------------------------------------------------------ */
/* Generate terms up to n (inclusive). */
static void extend_to(idx_t n)
{
    for (idx_t i = max_n + 1; i <= n; ++i) {
        ensure_capA(i);
        if (A[i] == (seq_t)0) {               /* undefined: assign new value */
            while (used[next_unused]) ++next_unused;
            seq_t val = (seq_t)next_unused;
            A[i] = val;
            ensure_cap_used(next_unused);
            used[next_unused] = 1;
        }
        seq_t val    = A[i];
        idx_t p = i + 1 + (idx_t)val;          /* position to copy to */
        ensure_capA(p);
        if (A[p] == (seq_t)0) {
            A[p] = val;                       /* copy forward if undefined */
        }
    }
    max_n = n;
}

/* ------------------------------------------------------------------ */
/* public function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* under‑offset undefined */

    /* lazily initialise cache on first call */
    if (A == NULL) {
        A = (seq_t *)calloc(1, 1);        /* dummy element at index 0 */
        capA = 0;
        used = (uint8_t *)calloc(1, 1);   /* value 0 unused */
        cap_used = 0;
        next_unused = 1;
    }

    if (n > max_n) extend_to(n);
    return A[n];
}
