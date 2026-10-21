/*
 * A222059  a(n) = n-th harmonic-exponential number, multiplied by n!.
 * https://oeis.org/A222059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=1..n} S(n,k) * H(k)/k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where S(n,k) are Stirling numbers of the second kind. */

#include <stdlib.h>

/* Ensure a dynamically allocated array is large enough for new_cap elements. */
static void ensure_cap(seq_t **arr, size_t *cap, size_t new_cap) {
    if (*cap < new_cap) {
        seq_t *tmp = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n) {
    /* Static state to memoise computed values */
    static idx_t          computed   = 0;   /* highest index computed so far */
    static seq_t *fact    = NULL;          /* n!                      */
    static seq_t *hfact   = NULL;          /* H(n)*n!                 */
    static seq_t *stirling = NULL;         /* S(n,k) (current row only)*/
    static seq_t *arr      = NULL;         /* cached a(n) values      */
    static size_t fact_cap   = 0,
                 hfact_cap  = 0,
                 stirling_cap = 0,
                 arr_cap    = 0;

    if (n == 0u)
        return 0;   /* a(0) = 0 per the offset */

    /* initialise arrays on first use */
    if (!fact) {
        size_t init_cap = n + 1;
        ensure_cap(&fact,   &fact_cap,   init_cap);
        ensure_cap(&hfact,  &hfact_cap,  init_cap);
        ensure_cap(&stirling, &stirling_cap, init_cap);
        ensure_cap(&arr,    &arr_cap,    init_cap);

        fact[0]    = 1;
        hfact[0]   = 0;
        stirling[0] = 1;   /* S(0,0)=1 */
        arr[0]     = 0;
        computed   = 0;
    }

    /* enlarge capacity if necessary */
    if (n + 1 > fact_cap) {
        size_t new_cap = n + 1;
        ensure_cap(&fact,   &fact_cap,   new_cap);
        ensure_cap(&hfact,  &hfact_cap,  new_cap);
        ensure_cap(&stirling, &stirling_cap, new_cap);
        ensure_cap(&arr,    &arr_cap,    new_cap);
    }

    if (n > computed) {
        /* compute values for indices old_computed+1 .. n */
        for (idx_t i = computed + 1; i <= n; ++i) {
            /* factorial: fact[i] = fact[i-1] * i */
            fact[i] = fact[i-1] * (seq_t)i;

            /* hfact[i] = H(i)*fact[i]   recurrence: hfact[i] = hfact[i-1]*i + fact[i-1] */
            hfact[i] = hfact[i-1] * (seq_t)i + fact[i-1];

            /* update Stirling numbers S(i,k) in place */
            stirling[i] = 0;                      /* pre‑set S(i,i) */
            for (idx_t k = i; k > 0; --k)
                stirling[k] = (seq_t)k * stirling[k] + stirling[k-1];
            stirling[0] = 0;                      /* S(i,0)=0 for i>0 */

            /* compute a(i) = fact[i] * Σ_{k=1..i} S(i,k) * H(k)/k! */
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k) {
                seq_t mult = fact[i] / fact[k];   /* integer division */
                mult *= stirling[k];
                seq_t term = mult * hfact[k];
                sum += term;
            }
            arr[i] = sum;
        }
        computed = n;
    }

    return arr[n];
}
