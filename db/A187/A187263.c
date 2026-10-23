/*
 * A187263  Number of nonempty subsets of {1, 2, ..., n} with <=2 pairwise
 *          coprime elements.
 * https://oeis.org/A187263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + Σ_{k=2..n} φ(k) = n + (Σ_{k=1..n} φ(k) – 1)
 * Range:     u8 n <= 26, u16 n <= 462, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count nonempty subsets of {1,…,n} having at most two pairwise coprime elements. */

#include <stdlib.h>

static seq_t *phi_arr = NULL;   /* φ values 0..n */
static seq_t *pref_arr = NULL;  /* prefix sums of φ */
static idx_t cur_cap = 0;       /* current capacity (≥ n+1) */

/* Ensure phi_arr[0..n] and pref_arr[0..n] are computed */
static void ensure_phi_pref(idx_t n)
{
    if (cur_cap >= n + 1)
        return;

    idx_t new_cap = n + 1;
    seq_t *new_phi;
    seq_t *new_pref;

#ifdef __cplusplus
    new_phi = new seq_t[new_cap];
    new_pref = new seq_t[new_cap];
#else
    new_phi = (seq_t *)malloc((size_t)new_cap * sizeof(seq_t));
    new_pref = (seq_t *)malloc((size_t)new_cap * sizeof(seq_t));
#endif
    if (!new_phi || !new_pref) {
        abort(); /* allocation failure */
    }

    /* initialise φ */
    new_phi[0] = (seq_t)0;
    if (new_cap > 1)
        new_phi[1] = (seq_t)1;
    for (idx_t i = 2; i < new_cap; ++i)
        new_phi[i] = (seq_t)i;

    /* Euler's sieve for φ */
    for (idx_t i = 2; i < new_cap; ++i) {
        if (new_phi[i] == (seq_t)i) {           /* i is prime */
            for (idx_t j = i; j < new_cap; j += i) {
                new_phi[j] = new_phi[j] - new_phi[j] / (seq_t)i;
            }
        }
    }

    /* prefix sums of φ */
    new_pref[0] = (seq_t)0;
    for (idx_t i = 1; i < new_cap; ++i)
        new_pref[i] = new_pref[i - 1] + new_phi[i];

    /* free old memory */
    if (phi_arr) {
#ifdef __cplusplus
        delete[] phi_arr;
#else
        free(phi_arr);
#endif
    }
    if (pref_arr) {
#ifdef __cplusplus
        delete[] pref_arr;
#else
        free(pref_arr);
#endif
    }

    phi_arr = new_phi;
    pref_arr = new_pref;
    cur_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for n=0, return 0 for safety */
        return (seq_t)0;

    ensure_phi_pref(n);

    /* Σ_{k=2..n} φ(k) = pref_arr[n] – φ(1) = pref_arr[n] – 1 */
    seq_t sum_phi_minus_one = pref_arr[n] - (seq_t)1;
    return (seq_t)n + sum_phi_minus_one;
}
