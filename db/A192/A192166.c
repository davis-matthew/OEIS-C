/*
 * A192166  Monotonic ordering of nonnegative differences 7^i-4^j, for
 *          40>=i>=0, j>=0.
 * https://oeis.org/A192166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A192166: sort nonnegative differences 7^i − 4^j
   for 0 ≤ i ≤ 40, j ≥ 0 (offset 1). */
#include <stdlib.h>

/* comparison function for qsort */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* static storage for the pre‑computed, sorted unique terms */
static seq_t *values = NULL;
static size_t num_values = 0;
static int initialized = 0;

/* one‑time initialisation: generate all 7^i−4^j ≥ 0 (i≤40) that fit in seq_t,
   sort them and remove duplicates. */
static void init_sequence(void)
{
    if (initialized) return;
    initialized = 1;

    /* Start with a generous capacity; we only need ~1200 elements. */
    size_t cap = 1024;
    values = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!values) exit(EXIT_FAILURE);

    /* base = 7^i, incrementally computed until overflow or i==40. */
    seq_t base = 1ULL;   /* 7^0 = 1 */
    for (idx_t i = 0; i <= 40; ++i) {
        /* Generate all j with 4^j ≤ base. */
        seq_t pow4 = 1ULL;   /* 4^0 = 1 */
        while (1) {
            if (pow4 <= base) {
                /* store the nonnegative difference */
                if (num_values >= cap) {
                    cap *= 2;
                    values = (seq_t *)realloc(values, cap * sizeof(seq_t));
                    if (!values) exit(EXIT_FAILURE);
                }
                values[num_values++] = base - pow4;
            }

            /* Decide whether another 4^j will still be <= base and safe to compute. */
            if (pow4 > base / 4) break;         /* next multiply would exceed base */
            pow4 *= 4;
        }

        /* Attempt to compute the next power of 7. */
        seq_t next = base * 7;
        if (next < base) break;              /* unsigned overflow: stop generating */
        base = next;
    }

    /* Sort the collected values and keep only the unique ones. */
    qsort(values, num_values, sizeof(seq_t), cmp_seq);
    size_t uniq = 0;
    for (size_t i = 0; i < num_values; ++i) {
        if (i == 0 || values[i] != values[uniq - 1]) {
            values[uniq++] = values[i];
        }
    }
    num_values = uniq;
}

/* public interface required by the framework */
seq_t a(idx_t n)
{
    if (!initialized) init_sequence();
    if (n == 0) return (seq_t)0;          /* out of range; sequence is 1‑based */
    size_t idx = (size_t)(n - 1);
    if (idx < num_values) return values[idx];
    return (seq_t)0;                      /* out of range beyond pre‑computed terms */
}
