/*
 * A076992  Given a(1), ..., a(n-1), a(n) is minimal such that all terms of
 *          the sequence are distinct positive integers and, for all k>=1, the
 *          sum of the k terms from a(k) to a(2k-1) is a k-th power.
 * https://oeis.org/A076992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 11, u32 n <= 40, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Recurrence for OEIS A076992
 *
 *   a(1) = 1
 *   if n is even:  a(n) = smallest positive integer not yet used
 *   if n is odd (n >= 3): let k = (n+1)/2
 *                       a(n) = 3^k – Σ_{i=k}^{n-1} a(i)
 *
 *   The sequence is generated lazily with memoisation so that calls
 *   may arrive in any order.  All calculations are carried out in
 *   the type seq_t; the implementation takes care of dynamic
 *   allocation and casts required by the C++ compiler.
 */
#include <stdlib.h>
#include <stdbool.h>

/* Return the n‑th term of sequence A076992.  */
seq_t a(idx_t n)
{
    /* --- static cache ----------------------------------------------------- */
    static seq_t *seq        = NULL;   /* sequence values (0‑based)      */
    static seq_t *pref       = NULL;   /* prefix sums: pref[0] = 0       */
    static size_t len        = 0;      /* number of elements stored      */
    static size_t cap        = 0;      /* current capacity of seq/pref   */
    static bool  *used_small = NULL;   /* flag array for small values    */
    static size_t used_cap   = 0;      /* size of used_small array      */
    static idx_t  next_candidate = 2;  /* next smallest unused integer   */
    static seq_t pow3          = 3;    /* 3^current_k for odd n          */
    static bool   initialized = false;

    /* --- one‑time initialisation ------------------------------------------ */
    if (!initialized) {
        /* allow enough space for the smallest values used in even positions   */
        used_cap = 1U << 20;                           /* 1 048 576 entries               */
        used_small = (bool *)calloc(used_cap + 1, sizeof(bool));
        if (!used_small) exit(EXIT_FAILURE);
        used_small[1] = true;                          /* a(1) = 1 is already used       */

        cap = 16;
        seq  = (seq_t *)malloc(cap * sizeof(seq_t));
        pref = (seq_t *)malloc((cap + 1) * sizeof(seq_t));
        if (!seq || !pref) exit(EXIT_FAILURE);
        pref[0] = 0;

        /* store a(1) */
        seq[0] = 1;
        pref[1] = pref[0] + seq[0];
        len = 1;
        initialized = true;
    }

    /* --- ensure we have enough room for the requested index --------------- */
    while (len < n) {
        if (len == cap) {                           /* grow if full */
            size_t new_cap = cap * 2;
            seq_t *new_seq  = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!new_seq) exit(EXIT_FAILURE);
            seq = new_seq;

            seq_t *new_pref = (seq_t *)realloc(pref, (new_cap + 1) * sizeof(seq_t));
            if (!new_pref) exit(EXIT_FAILURE);
            pref = new_pref;

            cap = new_cap;
        }

        idx_t i = len + 1;               /* 1‑based index of the new term */
        seq_t val;

        if ((i & 1ULL) == 0) {           /* even n */
            /* "next_candidate" holds the smallest integer not yet used.      */
            while (next_candidate <= used_cap &&
                   used_small[(size_t)next_candidate])
                ++next_candidate;
            val = next_candidate;
            used_small[(size_t)val] = true;
            ++next_candidate;
        } else {                         /* odd n (>=3) */
            idx_t k = (i + 1) / 2;       /* k = (n+1)/2                    */
            pow3 = pow3 * (seq_t)3;      /* 3^k for this odd n             */
            /* sum a(k)..a(n-1) = pref[n-1] – pref[k-1] */
            seq_t sum = pref[(size_t)(i - 1)] - pref[(size_t)(k - 1)];
            val = pow3 - sum;
        }

        /* store the new value and update prefix sum */
        seq[len] = val;
        pref[(size_t)(len + 1)] = pref[(size_t)len] + val;

        /* mark the value in the small‑number table if it fits */
        if (val <= (seq_t)used_cap)
            used_small[(size_t)val] = true;

        ++len;                                 /* now we have one more term   */
    }

    return seq[n - 1];     /* seq is 0‑based, a(n) is at index n-1 */
}
