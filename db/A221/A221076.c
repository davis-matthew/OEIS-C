/*
 * A221076  Continued fraction expansion of product_{n>=0}
 *          (1-sqrt(5)*[sqrt(5)-2]^{4n+3})/(1-sqrt(5)*[sqrt(5)-2]^{4n+1}).
 * https://oeis.org/A221076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, cofr
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A221076 – continued fraction coefficients of a product.
   The sequence satisfies:
     a(0)             = 2
     a(2*n)           = 1   (n ≥ 1)
     a(4*k - 3)       = S[2*k] - 2
     a(4*k - 1)       = U[2*k + 1] - 2
   where the auxiliary sequences S and U obey
     S[0] = 2, S[1] = 4
     U[0] = 0, U[1] = 2
     S[i] = 4*S[i-1] + S[i-2]   (i ≥ 2)
     U[i] = 4*U[i-1] + U[i-2]   (i ≥ 2)
   See the comments in the OEIS entry for the derivation of these formulae. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *S_arr = NULL;
static seq_t *U_arr = NULL;
static idx_t  S_max = 0;          /* biggest index computed (inclusive) */
static idx_t  U_max = 0;
static idx_t  S_cap = 0;
static idx_t  U_cap = 0;

/* Ensure that S_arr and U_arr contain values up to at least 'need'. */
static void ensure_capacity(idx_t need)
{
    if (S_arr == NULL) {
        /* arbitrary initial capacity */
        S_cap = U_cap = (need < 16 ? 16 : need + 1);
        S_arr = (seq_t *)malloc(S_cap * sizeof(seq_t));
        U_arr = (seq_t *)malloc(U_cap * sizeof(seq_t));
        if (!S_arr || !U_arr) return;   /* ignore allocation failure */
        /* initialise first two terms */
        S_arr[0] = 2;
        U_arr[0] = 0;
        S_arr[1] = 4;
        U_arr[1] = 2;
        S_max = U_max = 1;
    }
    /* grow arrays if necessary */
    if (need > S_cap - 1) {
        idx_t new_cap = S_cap * 2;
        if (new_cap <= need) new_cap = need + 1;
        seq_t *tmp = (seq_t *)realloc(S_arr, new_cap * sizeof(seq_t));
        if (tmp) {
            S_arr = tmp;
            S_cap = new_cap;
        }
    }
    if (need > U_cap - 1) {
        idx_t new_cap = U_cap * 2;
        if (new_cap <= need) new_cap = need + 1;
        seq_t *tmp = (seq_t *)realloc(U_arr, new_cap * sizeof(seq_t));
        if (tmp) {
            U_arr = tmp;
            U_cap = new_cap;
        }
    }
    /* compute missing values */
    if (need > S_max) {
        for (idx_t i = S_max + 1; i <= need; ++i) {
            S_arr[i] = (seq_t)(4) * S_arr[i - 1] + S_arr[i - 2];
        }
        S_max = need;
    }
    if (need > U_max) {
        for (idx_t i = U_max + 1; i <= need; ++i) {
            U_arr[i] = (seq_t)(4) * U_arr[i - 1] + U_arr[i - 2];
        }
        U_max = need;
    }
}

seq_t a(idx_t n)
{
    /* special handling of small indices */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)16;
    if ((n & 1) == 0)  /* even index, n>=2 */
        return (seq_t)1;

    /* odd indices (n >= 3) */
    idx_t idx = (n + 3) / 2;          /* = floor((n+3)/2) */
    ensure_capacity(idx);

    if (n % 4 == 1)   /* n = 4*k - 3  */
        return S_arr[idx] - (seq_t)2;
    else              /* n = 4*k - 1 */
        return U_arr[idx] - (seq_t)2;
}
