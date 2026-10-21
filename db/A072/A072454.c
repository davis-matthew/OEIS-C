/*
 * A072454  Number of nontotients in the reduced residue system of 2n-1.
 * https://oeis.org/A072454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A072106(2*n-1) – count of nontotients in the reduced
 *            residue * system of 2*n-1. The implementation uses a precomputed
 *            totient table * large enough to cover all φ values up to 2*n-1
 *            for the given n
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/*  Euclidean GCD  ----------------------------------------------------*/
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ------------------------------------------------------------------ */
/*  Global tables for φ and totient existence ------------------------*/
static size_t phi_limit = 0;      /* current size of tables (>= 0)       */
static idx_t  *phi_arr   = NULL;  /* φ[i] for 0 <= i <= phi_limit      */
static bool    *is_totient_table = NULL; /* true if i is a totient ≤ phi_limit */

/* ------------------------------------------------------------------ */
/*  Ensure all φ(i) for i <= need are computed and all totient values
 *  up to need are marked.  We allocate tables of size need+1.  The
 *  precomputation uses a standard sieve. --------------------------------*/
static void ensure_precalc(size_t need)
{
    if (need <= phi_limit)
        return; /* already sufficient */

    /* free old data */
    free(phi_arr);
    free(is_totient_table);

    /* allocate new arrays */
    phi_arr = (idx_t *)malloc((need + 1) * sizeof(idx_t));
    if (!phi_arr) abort();

    /* initialize φ(i) = i */
    for (size_t i = 0; i <= need; ++i)
        phi_arr[i] = (idx_t)i;

    /* sieve to compute φ for all i <= need */
    for (size_t p = 2; p <= need; ++p) {
        if (phi_arr[p] == (idx_t)p) {           /* p is prime */
            for (size_t j = p; j <= need; j += p) {
                phi_arr[j] -= phi_arr[j] / (idx_t)p;
            }
        }
    }

    /* mark which integers up to need are totients */
    is_totient_table = (bool *)malloc((need + 1) * sizeof(bool));
    if (!is_totient_table) abort();

    for (size_t i = 0; i <= need; ++i)
        is_totient_table[i] = false;

    for (size_t i = 1; i <= need; ++i) {
        idx_t v = phi_arr[i];
        if (v <= (idx_t)need)
            is_totient_table[(size_t)v] = true;
    }

    phi_limit = need;
}

/* ------------------------------------------------------------------ */
/*  Sequence function a(n)                                            */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                           /* offset is 1, safety */

    idx_t m = 2 * n - 1;                   /* odd integer 2*n-1       */

    /* Ensure precomputation up to 4*m (sufficient for all φ <= 2*m-1) */
    size_t need = (size_t)(4 * m + 1);      /* +1 to be safe          */
    ensure_precalc(need);

    seq_t result = 0;

    /* Count coprime k < m that are not totients */
    for (idx_t k = 1; k < m; ++k) {
        if (gcd_idx(k, m) == 1 && !is_totient_table[(size_t)k])
            result += (seq_t)1;
    }

    return result;
}
