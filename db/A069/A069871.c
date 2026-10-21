/*
 * A069871  Numbers m that divide the concatenation of m-1 and m+1.
 * https://oeis.org/A069871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A069871: numbers m that divide the concatenation of m‑1 and m+1.
   The terms are the union of
        A_k = (10^k – 1)/3      , k ≥ 1           (…, 3, 33, 333, …)
        B_k = (10^k – 1)/9      , k ≥ 2           (…, 11, 111, …)
        C_k = (10^(6k) – 1)/7    , k ≥ 1           (…, 142857, …)
   plus the special value 9.  The three monotone families are merged
   to produce the sorted sequence.  The implementation keeps a static
   cache of terms and generates them on demand. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* 0‑based index used internally */
    idx_t idx = n;                      /* n ≥ 1 in all calls */

    /* ---------- static cache of generated values ---------- */
    static bool initialized = false;
    static seq_t *terms = NULL;
    static size_t len = 0, cap = 0;

    /* ---------- variables that describe the three families ---------- */
    static seq_t pow10A = 0;            /* current 10^k for family A   */
    static seq_t pow10B = 0;            /* current 10^k for family B   */
    static seq_t pow10C = 0;            /* current 10^(6k) for family C*/
    static const seq_t incPow6 = 0;     /* pre‑computed 10^6            */
    static bool overflowA = false;
    static bool overflowB = false;
    static bool overflowC = false;

    /* ---------- one‑time initialisation ---------- */
    if (!initialized)
    {
        /* minimal capacity for the first few values (3,9) */
        cap = 8;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!terms) return 0;          /* out of memory, return 0 */

        /* base powers for the families   */
        pow10A = pow10(1);             /* 10^1  → 10  → (10-1)/3 = 3   */
        pow10B = pow10(2);             /* 10^2  → 100 → (100-1)/9 = 11*/
        pow10C = pow10(6);             /* 10^6  → 1 000 000 → (10^6-1)/7 =142857 */

        /* 10^6 to advance the C‑family */
        const seq_t tmp_inc = pow10(6);
        /* Store into static const (needs compound literal) */
        /* because compilers may not allow modifying a const variable,
           we keep it in a non‑const static variable. */
        /* Here, we duplicate the value in a normal variable. */
        /* (The constant is really small, so using a normal variable is fine.) */
        /* We'll use a local variable for multiply operations. */
        /* The static incPow6 is effectively read-only after init. */

        /* Insert the first two terms explicitly: 3 and 9 */
        terms[0] = (pow10(1) - 1) / 3;  /* 3   */
        terms[1] = 9;                   /* 9   */
        len = 2;

        initialized = true;
    }

    /* Helper for growing the cache */
    if (len < idx)
    {
        const seq_t incPow6_val = pow10(6);

        /* We will need to refer to incPow6 now, so keep a local copy. */
        seq_t incPow6_local = incPow6_val;

        /* The subsequent loop keeps generating until len ≥ idx. */
        while (len < idx)
        {
            /* Current candidates from each family (if not overflowed) */
            seq_t candA = overflowA ? (seq_t)-1 : (pow10A - 1) / 3;
            seq_t candB = overflowB ? (seq_t)-1 : (pow10B - 1) / 9;
            seq_t candC = overflowC ? (seq_t)-1 : (pow10C - 1) / 7;

            /* Pick the smallest candidate */
            seq_t next = candA;
            int which = 0;               /* 0 = A, 1 = B, 2 = C */
            if (candB < next) { next = candB; which = 1; }
            if (candC < next) { next = candC; which = 2; }

            /* Add to cache if this is a new, strictly larger value */
            if (len == 0 || next > terms[len - 1])
            {
                if (len == cap)
                {
                    size_t newcap = cap * 2;
                    seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
                    if (!tmp) return 0;  /* out of memory */
                    terms = tmp;
                    cap = newcap;
                }
                terms[len++] = next;
            }

            /* Advance the chosen family, taking care of overflow on 64‑bit builds */
            if (which == 0 && !overflowA)
            {
                if (sizeof(seq_t) <= 8) /* 64‑bit build */
                {
                    if (pow10A > ((seq_t)UINT64_MAX) / 10) overflowA = true;
                    else pow10A *= 10;
                }
                else pow10A *= 10;
            }
            else if (which == 1 && !overflowB)
            {
                if (sizeof(seq_t) <= 8)
                {
                    if (pow10B > ((seq_t)UINT64_MAX) / 10) overflowB = true;
                    else pow10B *= 10;
                }
                else pow10B *= 10;
            }
            else if (which == 2 && !overflowC)
            {
                if (sizeof(seq_t) <= 8)
                {
                    if (pow10C > ((seq_t)UINT64_MAX) / incPow6_local)
                        overflowC = true;
                    else pow10C *= incPow6_local;
                }
                else pow10C *= incPow6_local;
            }
        }
    }

    return terms[idx - 1];
}
