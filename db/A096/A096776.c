/*
 * A096776  n / Max{a(k): 1<=k<n and a(k) divides n}, a(1) = 1.
 * https://oeis.org/A096776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / max{ a(k) : 1 <= k < n and a(k) divides n }
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence by dynamic programming: for each n it keeps a list
   of already seen a(k) values.  The largest divisor of n that has been
   seen gives the required maximum.  The implementation is thread‑safe
   because all static data are local to this translation unit. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static seq_t *terms = NULL;      /* a(0) unused, a[1] … a[max] */
static bool *seen_val = NULL;    /* seen_val[v] == true if some a(k)=v seen so far */
static size_t terms_cap = 0;     /* capacity of terms array */
static size_t seen_cap  = 0;     /* capacity of seen_val array */
static idx_t  computed_until = 0;/* highest index already computed */

/* Ensure that arrays are large enough for indices up to n. */
static void ensure_capacity(idx_t n)
{
    /* Allocate terms array */
    if (terms_cap < (size_t)n + 1) {
        size_t new_cap = terms_cap ? terms_cap * 2 : (n + 1) * 2;
        if (new_cap <= terms_cap) new_cap = n + 1; /* guard against overflow */
        terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        terms_cap = new_cap;
    }
    /* Allocate seen_val array and initialise new entries to false */
    if (seen_cap < (size_t)n + 1) {
        size_t new_cap = seen_cap ? seen_cap * 2 : (n + 1) * 2;
        if (new_cap <= seen_cap) new_cap = n + 1;
        bool *tmp = (bool*)realloc(seen_val, new_cap * sizeof(bool));
        seen_val = tmp;
        if (seen_cap == 0)
            memset(seen_val, 0, new_cap * sizeof(bool));
        else
            memset(seen_val + seen_cap, 0, (new_cap - seen_cap) * sizeof(bool));
        seen_cap = new_cap;
    }
}

extern seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n == 0, but guard anyway */

    /* Initialise arrays on first call */
    if (computed_until == 0) {
        ensure_capacity(n);
        /* a(1) is defined as 1 */
        seen_val[1] = true;
        terms[1] = (seq_t)1;
        computed_until = 1;
    }

    /* Compute any missing terms up to n */
    if (computed_until < n) {
        ensure_capacity(n);

        for (idx_t m = computed_until + 1; m <= n; ++m) {
            idx_t largest = 1;   /* 1 has already been seen */
            for (idx_t d = 1; d * d <= m; ++d) {
                if (m % d == 0) {
                    idx_t cand1 = d;
                    idx_t cand2 = m / d;
                    if (seen_val[cand1] && cand1 > largest) largest = cand1;
                    if (seen_val[cand2] && cand2 > largest) largest = cand2;
                }
            }
            seq_t val = ((seq_t)m) / ((seq_t)largest);
            terms[m] = val;
            if (val <= (seq_t)n) seen_val[(size_t)val] = true;
        }
        computed_until = n;
    }

    return terms[n];
}
