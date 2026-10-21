/*
 * A072455  Number of totients in the reduced residue system of 2n-1.
 * https://oeis.org/A072455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of totients in the reduced residue system of 2*n-1
 *            a(n) = φ(2*n-1) - A072454(n) (where A072454 counts nontotients
 *            in that system)
 * Range:     u8 n <= 32, u16 n <= 8151, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t *phi_arr = NULL;          /* φ(i) for i ≤ max_limit      */
static unsigned char *totient_flag = NULL;  /* flag[v] == 1 if v is a totient */
static idx_t max_limit = 0;            /* largest i currently precomputed */

/* greatest common divisor for idx_t */
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Ensure tables cover all numbers up to a safe bound for the current n.
   The bound is 4*(2*n-1) to cover all residues (≤ 2n-1) that can be totient
   values of integers up to that bound. */
static void ensure_precalc(idx_t n)
{
    idx_t m = 2 * n - 1;
    idx_t needed = m * 4;
    if (needed < 10) needed = 10;           /* small safe default */
    if (needed <= max_limit)
        return;                            /* already prepared */

    /* allocate new arrays */
    seq_t *new_phi = (seq_t *)malloc((needed + 1) * sizeof(seq_t));
    unsigned char *new_flag = (unsigned char *)malloc((needed + 1) * sizeof(unsigned char));
    if (!new_phi || !new_flag) {
        /* memory exhaustion – abort */
        exit(EXIT_FAILURE);
    }

    /* initialise: φ(i)=i, flag=0 */
    for (idx_t i = 0; i <= needed; ++i) {
        new_phi[i] = (seq_t)i;
        new_flag[i] = 0;
    }

    /* sieve for φ */
    for (idx_t i = 2; i <= needed; ++i) {
        if (new_phi[i] == (seq_t)i) {           /* i is prime */
            for (idx_t j = i; j <= needed; j += i) {
                new_phi[j] -= new_phi[j] / (seq_t)i;
            }
        }
    }

    /* mark totient values */
    for (idx_t i = 1; i <= needed; ++i) {
        seq_t v = new_phi[i];
        if (v <= needed)
            new_flag[(size_t)v] = 1;
    }

    /* free old arrays */
    if (phi_arr) free(phi_arr);
    if (totient_flag) free(totient_flag);

    phi_arr = new_phi;
    totient_flag = new_flag;
    max_limit = needed;
}

/* ------------------------------------------------------------------ */
/* Sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* below offset – harmless */

    if (n == 1)                  /* definition of a(1) = 1 */
        return (seq_t)1;

    ensure_precalc(n);

    idx_t m = 2 * n - 1;
    seq_t count = 0;

    for (idx_t k = 1; k < m; ++k) {
        if (gcd_idx(k, m) == 1) {                 /* k in reduced residue system */
            if (k <= max_limit && totient_flag[(size_t)k])
                ++count;                          /* k is a totient value */
        }
    }
    return count;
}
