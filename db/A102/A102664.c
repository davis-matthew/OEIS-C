/*
 * A102664  Lexically least sequence of distinct positive integers such that
 *          for all j and k, 1+a(j)-a(k) is 0, 1 or not a square.
 * https://oeis.org/A102664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   isqrt, seq_malloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexically least sequence of distinct positive integers such that for all
   j,k: 1 + a(j) - a(k) is 0, 1 or not a square.  This implementation
   builds the sequence incrementally, caching results so that calls in any
   order are handled efficiently.  The function uses dynamic allocation
   compatible with both fixed‑width integer types and the arbitrary‑
   precision type provided by the build. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/isqrt.h"     /* provides seq_t isqrt(seq_t) */
#include "../utils/seq_malloc.h"/* provides seq_t *seq_malloc(idx_t)  */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1, keep zero */

    /* ---------- dynamic storage for sequence ---------- */
    static seq_t *seq     = NULL;
    static idx_t seq_len  = 0;
    static idx_t seq_cap  = 0;

    /* ---------- used flag array for candidate integers ---------- */
    static bool *used     = NULL;
    static idx_t used_cap = 0;

    /* ---------- small squares cache (for fast look‑up) ---------- */
    static bool *sq_flag  = NULL;
    static idx_t sq_cap   = 0;
    static bool sq_init_done = false;

    /* ---------- one‑time initialisation ---------- */
    if (!sq_init_done)
    {
        /* capacity for the used array and the sequence array */
        seq_cap  = 64;
        seq      = seq_malloc(seq_cap);

        used_cap = 16;
        used     = (bool *)malloc(used_cap * sizeof(bool));
        for (idx_t i = 0; i < used_cap; ++i) used[i] = false;

        /* precompute squares up to some limit.
           The differences between terms for n <= 10000 stay far below 2^20
           (about 1M), so 1<<20 is safe. */
        sq_cap   = 1 << 20;
        sq_flag  = (bool *)malloc(sq_cap * sizeof(bool));
        for (idx_t i = 0; i < sq_cap; ++i) sq_flag[i] = false;
        for (idx_t i = 1;; ++i) {
            idx_t sq = i * i;
            if (sq >= sq_cap) break;
            sq_flag[sq] = true;
        }
        sq_init_done = true;
    }

    /* ---------- grow array if unused ---------- */
    while (seq_len < n) {
        /* ++ find next term */
        idx_t cand = 1;
        for (;;) {
            /* enlarge the used array if required */
            if (cand >= used_cap) {
                idx_t new_cap = used_cap * 2;
                bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
                for (idx_t i = used_cap; i < new_cap; ++i) tmp[i] = false;
                used = tmp;
                used_cap = new_cap;
            }
            if (used[cand]) { cand++; continue; }

            bool conflict = false;
            for (idx_t j = 0; j < seq_len && !conflict; ++j) {
                seq_t tj = seq[j];
                /* compute |cand - tj| */
                long long d = (long long)cand - (long long)tj;
                seq_t sum;
                if (d >= 0)
                    sum = (seq_t)(d + 1ULL);
                else
                    sum = (seq_t)((-d) + 1ULL);

                bool square;
                if (sum < sq_cap)
                    square = sq_flag[(idx_t)sum];
                else {
                    seq_t r = isqrt(sum);
                    square = r * r == sum;
                }
                if (square) conflict = true;
            }
            if (!conflict) break;  /* cand is the next term */
            cand++;
        }

        /* ++ store the new term in the sequence array */
        if (seq_len == seq_cap) {
            idx_t new_cap = seq_cap * 2;
            seq_t *tmp = seq_malloc(new_cap);
            for (idx_t i = 0; i < seq_len; ++i) tmp[i] = seq[i];
            seq = tmp;
            seq_cap = new_cap;
        }
        seq[seq_len++] = (seq_t)cand;

        /* mark cand as used */
        used[cand] = true;
    }

    return seq[n - 1];
}
