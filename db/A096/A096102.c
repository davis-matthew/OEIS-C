/*
 * A096102  a(1) = 1, a(2) = 3; for n > 2: a(n) = smallest (odd) number not
 *          occurring earlier such that the sum of each section of odd length
 *          >= 3 is prime.
 * https://oeis.org/A096102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = smallest odd integer not used earlier such that all *
 *            odd‑length contiguous suffix sums (≥3) are prime. * Initial
 *            terms: a(1)=1, a(2)=3. The sequence ends after a(6)=13
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#if defined(__cplusplus)
#   include <cstring>
#else
#   include <stdlib.h>
#endif
#include "../utils/isPrimeGeneric.h"

/* static state */
static seq_t *seq_arr    = NULL;   /* terms (0‑based)            */
static seq_t *pref_arr   = NULL;   /* running sums of seq_arr    */
static idx_t  seq_len    = 0;      /* number of terms stored     */
static idx_t  capacity   = 0;      /* allocated capacity          */

/* Ensure that 'need' elements can be stored. */
#if defined(__cplusplus)
/* C++: use new[] and element copy. */
static void ensure_capacity(idx_t need)
{
    if (need <= capacity) return;
    idx_t new_cap = capacity ? capacity * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *new_arr  = new seq_t[new_cap];
    seq_t *new_pref = new seq_t[new_cap];
    for (idx_t i = 0; i < seq_len; ++i) {
        new_arr[i]  = seq_arr[i];
        new_pref[i] = pref_arr[i];
    }
    delete[] seq_arr;
    delete[] pref_arr;
    seq_arr  = new_arr;
    pref_arr = new_pref;
    capacity = new_cap;
}
#else
/* C: use realloc (truncating) */
static void ensure_capacity(idx_t need)
{
    if (need <= capacity) return;
    idx_t new_cap = capacity ? capacity * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *new_arr  = (seq_t *)realloc(seq_arr,  new_cap * sizeof(seq_t));
    seq_t *new_pref = (seq_t *)realloc(pref_arr, new_cap * sizeof(seq_t));
    seq_arr  = new_arr;
    pref_arr = new_pref;
    capacity = new_cap;
}
#endif

/* Return the n‑th term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for 0 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n > 6)  return 0;            /* sequence terminates after a(6) */

    /* initialise with first two terms if necessary */
    if (!seq_arr) {
        ensure_capacity(6);          /* allocate enough space for at most 6 terms */
        seq_arr[0]    = (seq_t)1;
        seq_arr[1]    = (seq_t)3;
        pref_arr[0]   = seq_arr[0];
        pref_arr[1]   = seq_arr[0] + seq_arr[1];
        seq_len       = 2;
    }

    /* generate terms until we have at least n terms */
    for (; seq_len < n; ++seq_len) {
        /* find the smallest admissible odd candidate */
        seq_t cand = (seq_t)1;        /* start from 1 (odd); 1 is already used */
        for (;;) {
            /* ensure odd */
            if ((cand & 1u) == 0) { cand += 2; continue; }

            /* skip values already used */
            bool used = false;
            for (idx_t j = 0; j < seq_len; ++j) {
                if (seq_arr[j] == cand) { used = true; break; }
            }
            if (used) { cand += 2; continue; }

            /* check prime sums for all odd‑length suffixes (≥3) */
            bool ok = true;
            seq_t prefix_last = pref_arr[seq_len - 1];
            for (idx_t L = 3; L <= seq_len + 1; L += 2) {
                idx_t start = (seq_len + 1) - L;        /* start index of suffix */
                seq_t old_sum = (start == 0) ?
                    prefix_last : (prefix_last - pref_arr[start - 1]);
                seq_t sum = old_sum + cand;
                if (!isPrimeGeneric(sum)) { ok = false; break; }
            }
            if (ok) break;                 /* candidate accepted */
            cand += 2;                      /* try next odd number */
        }

        /* append accepted candidate */
        ensure_capacity(seq_len + 1);
        seq_arr[seq_len] = cand;
        pref_arr[seq_len] = pref_arr[seq_len - 1] + cand;
    }

    return seq_arr[n - 1];
}
