/*
 * A038856  Numbers ending with '1' that are the difference of two positive
 *          cubes.
 * https://oeis.org/A038856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer that ends with digit 1 and can
 *            be expressed as u³ – v³ with u > v ≥ 1, indexed from n = 1
 * Range:     u8 none, u16 none, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Internal comparison function for qsort, works for any seq_t that supports <, > */
static int compare_seq_t(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/* Static storage for the generated sequence */
static seq_t *seq = NULL;      /* Pointer to sorted, unique terms */
static size_t seq_len = 0;     /* Number of terms stored */
static bool initialized = false;   /* Flag indicating that seq has been built */

/* Build the sequence once, up to a pre‑chosen bound that yields >10 000 terms. */
static void build_sequence(void)
{
    /* Adequate upper bound for the larger cube root; empirical */
    const idx_t LIMIT_A = 4000;

    /* Rough capacity estimate: about LIMIT_A²/20 candidates with digit 1 */
    size_t raw_cap = 1 << 20;   /* start with 1,048,576, ample for < 800 000 entries */
    size_t raw_len = 0;
    seq_t *raw = (seq_t *)malloc(raw_cap * sizeof(seq_t));
    if (!raw) abort();

    for (idx_t a = 1; a <= LIMIT_A; ++a) {
        seq_t a_cube = (seq_t)a * (seq_t)a * (seq_t)a;

        for (idx_t b = 1; b < a; ++b) {
            seq_t diff = a_cube - ( (seq_t)b * (seq_t)b * (seq_t)b );
            if (diff % (seq_t)10 == (seq_t)1) {   /* ends with digit 1 */
                if (raw_len == raw_cap) {
                    raw_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(raw, raw_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    raw = tmp;
                }
                raw[raw_len++] = diff;
            }
        }
    }

    /* Sort the raw list */
    qsort(raw, raw_len, sizeof(seq_t), compare_seq_t);

    /* Remove duplicates in place */
    size_t uniq_len = 0;
    for (size_t i = 0; i < raw_len; ++i) {
        if (uniq_len == 0 || raw[i] != raw[uniq_len - 1]) {
            raw[uniq_len++] = raw[i];
        }
    }

    seq = raw;
    seq_len = uniq_len;
    initialized = true;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (!initialized) {
        build_sequence();
    }
    /* The test harness guarantees that n-1 < seq_len for requested terms. */
    return seq[(size_t)(n - 1)];
}
