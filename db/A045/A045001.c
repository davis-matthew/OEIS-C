/*
 * A045001  Numbers whose base-3 representation contains exactly two 0's and
 *          two 2's.
 * https://oeis.org/A045001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers whose base‑3 representation contains exactly two 0's and
 *            two 2's
 * Range:     u8 n <= 21, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence starts at a(1) and is generated on demand. */

#include <stdlib.h>
#include <stdbool.h>

/* Global cache of computed terms. */
static seq_t *global_cache   = NULL; /* pointer to stored terms */
static size_t global_size    = 0;    /* number of terms currently cached */
static size_t global_cap     = 0;    /* allocated capacity of the cache */

/* Range of lengths processed so far (minimal length with two 0's and two 2's is 4). */
static int last_len = 3; /* will be incremented to 4 on first use */

/* Temporary buffer for generating a single length. */
static seq_t *temp_buf  = NULL;
static size_t temp_cap  = 0;

/* Comparison function for qsort. */
static int cmp_seq(const void *a, const void *b) {
    const seq_t va = *(const seq_t *)a;
    const seq_t vb = *(const seq_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* Generate all base‑3 numbers of length L (L ≥ 4) that contain exactly two 0's
 * and exactly two 2's, with the most significant digit non‑zero,
 * and append them to the global cache in ascending order. */
static void generate_len(int L) {
    if (L < 4) return;

    /* Maximum number of combinations for this length:
       choose two zero positions (excluding the first digit) and
       two two positions from the remaining slots. */
    const size_t combos = ((size_t)(L - 1) * (L - 2)) / 2 *
                          ((size_t)(L - 2) * (L - 3)) / 2;

    if (combos > temp_cap) {
        temp_cap = combos;
        temp_buf = (seq_t *)realloc(temp_buf, temp_cap * sizeof(seq_t));
        if (!temp_buf) abort();
    }

    size_t temp_sz = 0;
    bool zero[16] = {false};
    bool two[16]  = {false};

    for (int i = 0; i < L; ++i) {
        for (int j = i + 1; j < L; ++j) {
            /* The first digit cannot be zero. */
            if (i == 0 || j == 0) continue;
            zero[i] = zero[j] = true;

            for (int k = 0; k < L; ++k) {
                if (zero[k]) continue;
                for (int l = k + 1; l < L; ++l) {
                    if (zero[l]) continue;
                    two[k] = two[l] = true;

                    /* Compute the numeric value of the constructed base‑3 number. */
                    seq_t val = 0;
                    for (int pos = 0; pos < L; ++pos) {
                        int d = zero[pos] ? 0 : (two[pos] ? 2 : 1);
                        val = val * 3 + d;
                    }
                    temp_buf[temp_sz++] = val;

                    two[k] = two[l] = false;
                }
            }
            zero[i] = zero[j] = false;
        }
    }

    /* Sort the values of this length. */
    qsort(temp_buf, temp_sz, sizeof(seq_t), cmp_seq);

    /* Append them to the global cache. */
    for (size_t idx = 0; idx < temp_sz; ++idx) {
        if (global_size == global_cap) {
            size_t new_cap = global_cap ? global_cap * 2 : 16;
            global_cache = (seq_t *)realloc(global_cache, new_cap * sizeof(seq_t));
            if (!global_cache) abort();
            global_cap = new_cap;
        }
        global_cache[global_size++] = temp_buf[idx];
    }
}

/* Public function: a(n) for n ≥ 1 (offset 1).  Undefined for n == 0. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                           /* harmless, not part of the sequence */

    /* Ensure we have computed at least n terms. */
    while (global_size < (size_t)n) {
        ++last_len;
        generate_len(last_len);
    }

    return global_cache[(size_t)n - 1];
}
