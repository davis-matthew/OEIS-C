/*
 * A045278  Numbers whose base-5 representation contains no 2's and exactly
 *          one 4.
 * https://oeis.org/A045278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   generate all numbers whose base‑5 representation contains * no
 *            2’s and exactly one 4. The numbers are produced in increasing *
 *            order by length and lexicographic order of digits
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* cache of generated terms         */
static size_t cache_size = 0;   /* how many terms have been cached */

/* Limit for the current generation run  (used as a cut‑off in recursion). */
static idx_t generate_limit;

/* Recursive generator that fills 'out' with numbers of exactly length L.
 * 'pos'    : current position (0 = most significant digit).
 * 'placed4': 0 if the digit 4 has not yet been used; 1 otherwise.
 * 'cur'    : value built so far.
 * 'cnt'    : number of terms already stored (updated by reference). */
static void gen_len(idx_t L,
                    idx_t pos,
                    idx_t placed4,
                    seq_t cur,
                    seq_t *out,
                    idx_t *cnt)
{
    if (pos == L) {                     /* number finished            */
        if (placed4)                    /* exactly one 4 ?          */
            out[(*cnt)++] = cur;
        return;
    }

    if (pos == 0) {                     /* most significant digit   */
        int digits[3] = {1, 3, 4};
        for (int i = 0; i < 3; ++i) {
            int d = digits[i];
            idx_t new_placed4 = placed4 | (d == 4);
            seq_t next = cur * 5 + d;
            gen_len(L, pos + 1, new_placed4, next, out, cnt);
            if (*cnt >= generate_limit) return;
        }
    } else {                            /* other positions           */
        int digits[4] = {0, 1, 3, 4};
        for (int i = 0; i < 4; ++i) {
            int d = digits[i];
            if (placed4 && d == 4) continue;  /* second 4 forbidden       */
            idx_t new_placed4 = placed4 | (d == 4);
            seq_t next = cur * 5 + d;
            gen_len(L, pos + 1, new_placed4, next, out, cnt);
            if (*cnt >= generate_limit) return;
        }
    }
}

/* Generate the first 'limit' terms of the sequence into 'out'.  All
 * output values are stored in ascending order. */
static void generate_terms(seq_t *out, idx_t limit)
{
    generate_limit = limit;
    idx_t count = 0;
    for (idx_t L = 1; count < limit; ++L) {
        gen_len(L, 0, 0, 0, out, &count);
    }
}

/* Public function required by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* guard for misuse          */

    if (n > cache_size) {            /* grow cache and generate   */
        size_t new_size = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        cache = new_cache;
        generate_terms(cache, n);
        cache_size = n;
    }
    return cache[n - 1];
}
