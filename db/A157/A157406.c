/*
 * A157406  The integer partitions of n taken as digits in base n+1 and listed
 *          in the Hindenburg order.
 * https://oeis.org/A157406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 42, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates OEIS A157406 by enumerating the partitions of n in Hindenburg order
   and converting each partition to a base-(n+1) number.  The sequence is
   built lazily, extending the stored table until the requested index is
   reached. */

#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Static storage for the sequence values */
#ifdef __cplusplus
static std::vector<seq_t> seq_vec;          /* C++: dynamic vector */
#else
static seq_t *seq_buf = NULL;              /* C: dynamic array */
static size_t seq_len = 0;
static size_t seq_cap = 0;
#endif

/* Number of rows already generated; next row to generate is next_row. */
static idx_t next_row = 0;

/* Append a value to the static sequence storage. */
static void append_seq(seq_t val)
{
#ifdef __cplusplus
    seq_vec.push_back(val);
#else
    if (seq_len == seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 1 : seq_cap << 1;
        seq_t *newp = (seq_t *)realloc(seq_buf, new_cap * sizeof(seq_t));
        if (!newp) abort();   /* cannot recover from out‑of‑memory */
        seq_buf = newp;
        seq_cap = new_cap;
    }
    seq_buf[seq_len++] = val;
#endif
}

/* Generate all partitions of m in Hindenburg order and append their
   base-(m+1) representations to the sequence storage. */
static void generate_row(idx_t m)
{
    if (m == 0) {
        append_seq(0);
        return;
    }
    /* Allocate array for at most m+1 parts.  Use a C++ cast for C++ mode. */
    int *parts = (int *)malloc((size_t)(m + 1) * sizeof(int));
    if (!parts) abort();

    int k = 0;          /* last used index */
    parts[0] = (int)m;  /* start with the partition [m] */
    bool finished = false;

    while (!finished) {
        /* Convert current partition to a base-(m+1) number. */
        seq_t val = 0;
        seq_t base = (seq_t)(m + 1);
        for (int i = 0; i <= k; ++i)
            val = val * base + (seq_t)parts[i];
        append_seq(val);

        /* Produce the next partition. */
        int rem = 0;
        while (k >= 0 && parts[k] == 1) {
            rem += 1;
            --k;
        }
        if (k < 0) {
            finished = true;
            break;
        }
        parts[k]--;      /* decrease the last part > 1 */
        ++rem;           /* a 1 has been taken from the tail */
        while (rem > parts[k]) {
            parts[k + 1] = parts[k];
            rem -= parts[k];
            ++k;
        }
        parts[k + 1] = rem;
        ++k;
    }

    free(parts);
}

/* Public routine required by the specification. */
seq_t a(idx_t n)
{
    /* Extend the sequence until it contains at least n+1 elements. */
    for (;;) {
#ifdef __cplusplus
        if (seq_vec.size() > (size_t)n)
            return seq_vec[(size_t)n];
        /* Generate next row. */
#else
        if (seq_len > (size_t)n)
            return seq_buf[(size_t)n];
#endif
        generate_row(next_row);
        ++next_row;
    }
}
