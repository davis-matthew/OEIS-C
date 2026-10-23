/*
 * A140401  Let S be the set of numbers formed from the sum of three distinct
 *          elements of A140398, or the sum of three distinct elements of
 *          A140399, or the sum of three distinct elements of A140400;
 *          sequence gives complement of S.
 * https://oeis.org/A140401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) is the nth positive integer that can be written as one of
 *            the following six expressions, with all indices positive
 *            integers: F_k, F_k + F_{k-3}, F_k + F_{k-2}, F_{2k} + F_{2k-5},
 *            F_{2k+1} - F_{2k-4}, F_{2k+1} + F_{2k-3} where F_m is the m‑th
 *            Fibonacci number (F_1=1, F_2=1)
 * Range:     u8 n <= 32, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

typedef uint64_t idx_t;   /* provided by the build */

/* seq_t is defined by the build (uint8_t … uint64_t, unsigned __int128, or
   arbitrary precision integer class).  All operations are assumed to be
   available with +, -, *, /, % and comparisons. */
extern seq_t seq_from_str(const char *s);  /* dummy for typedef */

/*--- Helper to maintain a dynamic array of Fibonacci numbers ----------------*/
static seq_t *fib = NULL;      /* fib[1] = 1, fib[2] = 1, fib[0] unused */
static size_t fib_cap = 0, fib_len = 0;

/* Ensure fib has at least indices up to idx (1‑based). */
static void ensure_fib(idx_t idx) {
    while (fib_len <= (size_t)idx) {
        seq_t next;
        if (fib_len == 0) {               /* initial allocation */
            fib_cap = 16;
            fib = (seq_t*)malloc(fib_cap * sizeof(seq_t));
            fib[0] = 0;                   /* unused */
            fib[1] = 1;
            fib[2] = 1;
            fib_len = 3;
        } else {
            /* grow if necessary */
            if (fib_len == fib_cap) {
                fib_cap *= 2;
                fib = (seq_t*)realloc(fib, fib_cap * sizeof(seq_t));
            }
            next = fib[fib_len - 1] + fib[fib_len - 2];
            fib[fib_len] = next;
            fib_len++;
        }
    }
}

/* Compute value for a stream given its id, current k and ensure fib. */
static seq_t stream_val(int id, idx_t k) {
    switch (id) {
        case 1:                                   /* F_k */
            ensure_fib(k);
            return fib[k];
        case 2:                                   /* F_k + F_{k-3} (k>=4) */
            ensure_fib(k);
            ensure_fib(k - 3);
            return fib[k] + fib[k - 3];
        case 3:                                   /* F_k + F_{k-2} (k>=3) */
            ensure_fib(k);
            ensure_fib(k - 2);
            return fib[k] + fib[k - 2];
        case 4:                                   /* F_{2k} + F_{2k-5} (k>=3) */
            ensure_fib(2 * k);
            ensure_fib(2 * k - 5);
            return fib[2 * k] + fib[2 * k - 5];
        case 5:                                   /* F_{2k+1} - F_{2k-4} (k>=3) */
            ensure_fib(2 * k + 1);
            ensure_fib(2 * k - 4);
            return fib[2 * k + 1] - fib[2 * k - 4];
        case 6:                                   /* F_{2k+1} + F_{2k-3} (k>=2) */
            ensure_fib(2 * k + 1);
            ensure_fib(2 * k - 3);
            return fib[2 * k + 1] + fib[2 * k - 3];
        default:
            return 0;
    }
}

/*--- Cache to speed up repeated calls ------------------------------------*/
static idx_t cached_max = 0;
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* The function a() as required. */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined; safe guard */

    /* Ensure we have at least n cached terms. */
    if (n > cached_max) {
        /* Allocate or grow cache */
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        if (new_cap > cache_cap) {
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* Stream struct: id (1..6), current k, current value */
        struct Stream { int id; idx_t k; seq_t val; };
        struct Stream streams[6] = {
            {1, 1, 0},   /* S1: F_k, starts at k=1 */
            {2, 4, 0},   /* S2: F_k + F_{k-3}  (k>=4) */
            {3, 3, 0},   /* S3: F_k + F_{k-2}  (k>=3) */
            {4, 3, 0},   /* S4: F_{2k}+F_{2k-5} (k>=3) */
            {5, 3, 0},   /* S5: F_{2k+1}-F_{2k-4} (k>=3) */
            {6, 2, 0}    /* S6: F_{2k+1}+F_{2k-3} (k>=2) */
        };
        for (int i = 0; i < 6; ++i)
            streams[i].val = stream_val(streams[i].id, streams[i].k);

        seq_t last_val = 0;   /* to deduplicate */
        idx_t produced = 0;   /* number of distinct terms produced */

        while (produced < n) {
            /* Find stream with minimal current value */
            int min_i = 0;
            for (int i = 1; i < 6; ++i)
                if (streams[i].val < streams[min_i].val)
                    min_i = i;

            seq_t cur = streams[min_i].val;
            if (produced == 0 || cur != last_val) {
                produced++;
                cache[produced - 1] = cur;
                last_val = cur;
                if (produced == n) break;  /* we have the needed value */
            }
            /* Advance the chosen stream */
            streams[min_i].k++;
            streams[min_i].val = stream_val(streams[min_i].id, streams[min_i].k);
        }
        cached_max = n;
    }
    return cache[n - 1];
}
