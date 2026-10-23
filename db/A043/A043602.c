/*
 * A043602  Numbers whose base-5 representation has exactly 2 runs.
 * https://oeis.org/A043602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th smallest positive integer whose base-5
 *            representation has exactly 2 runs
 * Range:     u8 n <= 48, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Helper: simple insertion sort for seq_t arrays. */
static void sort_seq(seq_t *a, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            if (a[j] < a[i]) {
                seq_t t = a[i];
                a[i] = a[j];
                a[j] = t;
            }
        }
    }
}

seq_t a(idx_t n) {
    /* static state for memoisation */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;  /* number of generated terms */
    static size_t terms_cap = 0;  /* allocated capacity */
    static idx_t max_len = 1;     /* longest base-5 length already generated */

    if (n < 1) return 0;  /* undefined for n <= 0 */

    /* Generate terms until we have at least n entries */
    while (terms_len < (size_t)n) {
        /* generate numbers with base‑5 length max_len + 1 */
        ++max_len; /* new length L */
        if (max_len < 2) continue; /* skip length 1, not needed */

        size_t newcnt = 16 * (size_t)(max_len - 1); /* 4 * 4 * (L-1) */
        seq_t *buf = (seq_t*)malloc(newcnt * sizeof(seq_t));
        if (!buf) return 0; /* allocation failure */

        size_t idx = 0;
        for (idx_t d1 = 1; d1 <= 4; ++d1) {
            for (idx_t d2 = 0; d2 <= 4; ++d2) {
                if (d2 == d1) continue;
                for (idx_t k1 = 1; k1 <= max_len - 1; ++k1) {
                    idx_t k2 = max_len - k1;
                    seq_t val = 0;
                    for (idx_t i = 0; i < k1; ++i) {
                        val = val * (seq_t)5 + (seq_t)d1;
                    }
                    for (idx_t i = 0; i < k2; ++i) {
                        val = val * (seq_t)5 + (seq_t)d2;
                    }
                    buf[idx++] = val;
                }
            }
        }
        /* sort the newly generated numbers (they are all larger than previous ones) */
        sort_seq(buf, newcnt);

        /* Ensure capacity and append */
        if (terms_len + newcnt > terms_cap) {
            size_t newcap = terms_cap ? terms_cap << 1 : 256;
            while (terms_len + newcnt > newcap) newcap <<= 1;
            void *p = realloc(terms, newcap * sizeof(seq_t));
            if (!p) {
                free(buf);
                return 0;
            }
            terms = (seq_t*)p;
            terms_cap = newcap;
        }
        memcpy(terms + terms_len, buf, newcnt * sizeof(seq_t));
        terms_len += newcnt;
        free(buf);
    }

    return terms[(size_t)n - 1];
}
