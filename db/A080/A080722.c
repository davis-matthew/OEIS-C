/*
 * A080722  a(0) = 0; for n > 0, a(n) is taken to be the smallest positive
 *          integer greater than a(n-1) which is consistent with the condition
 *          "n is a term of the sequence if and only if a(n) == 1 (mod 3)".
 * https://oeis.org/A080722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A080722
   a(0) = 0
   For n > 0, a(n) is the smallest integer > a(n‑1) that is consistent
   with the self‑referential rule:
       "n is a term of the sequence if and only if a(n) ≡ 1 (mod 3)"
   The algorithm maintains a set of values that have already appeared
   (the “terms”) and checks the equivalence for every candidate.
 */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache of already computed a(n) values   */
    static seq_t *cache      = NULL;   /* cache[i] = a(i)          */
    static size_t cache_size = 0;      /* number of values cached  */
    static size_t cache_cap  = 0;      /* allocated capacity       */

    /* set of values that have already appeared in the sequence */
    static unsigned char *used     = NULL;   /* used[v] == 1 if v appears as a term */
    static size_t used_cap       = 0;        /* allocated capacity for used         */

    /* ---------- initialise on first call ---------- */
    if (cache == NULL) {
        cache_cap  = 64;
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0]   = (seq_t)0;
        cache_size = 1;

        used_cap  = 1;
        used      = (unsigned char *)malloc(used_cap * sizeof(unsigned char));
        used[0]   = 1;       /* 0 is a term (a(0))          */
    }

    /* ---------- grow cache while needed ---------- */
    while (cache_size <= (size_t)n) {
        /* we need to compute a(cache_size)   */
        seq_t prev = cache[cache_size - 1];
        seq_t cand = prev + (seq_t)1;

        /* index of the new element in the term set */
        size_t idx = cache_size;          /* this is n for the new term */

        /* old membership of index n before adding a(n) */
        bool old_in_set = (idx < used_cap) ? (used[idx] != 0) : false;

        /* --------- search for the smallest valid candidate ---------- */
        for (;;) {
            size_t cand_idx = (size_t)cand;

            /* grow used array if required to check membership of cand */
            if (cand_idx >= used_cap) {
                size_t new_cap = used_cap ? used_cap : 1;
                while (new_cap <= cand_idx) new_cap <<= 1;
                unsigned char *tmp = (unsigned char *)
                    realloc(used, new_cap * sizeof(unsigned char));
                if (!tmp) abort();                 /* allocation error */
                memset(tmp + used_cap, 0,
                       (new_cap - used_cap) * sizeof(unsigned char));
                used     = tmp;
                used_cap = new_cap;
            }

            /* reject if value already used */
            if (used[cand_idx])
                { cand++; continue; }

            /* new membership of n after adding a(n) */
            bool new_term = ((cand == (seq_t)idx) || old_in_set);

            /* must satisfy equivalence: a(n) ≡ 1 (mod 3)  iff  n is a term */
            bool mod_ok = ((cand % (seq_t)3) == (seq_t)1);

            if (new_term == mod_ok) {
                /* ---------- accept this candidate ---------- */
                /* grow cache if needed */
                if (cache_size >= cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
                    seq_t *tmp = (seq_t *)
                        realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_size++] = cand;
                used[cand_idx] = 1;          /* record that cand is now a term */
                break;
            }

            cand++;
        }
    }

    return cache[n];
}
