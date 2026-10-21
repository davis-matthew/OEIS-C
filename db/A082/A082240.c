/*
 * A082240  Break up the cyclic concatenation 1234567890123456789023... into
 *          parts nontrivially (a(n) is not equal to n) such that the n-th
 *          part is a multiple of n.
 * https://oeis.org/A082240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) is defined greedily as the smallest multiple of n not equal
 *            to n such that the part does not start with 0 and the next part
 *            will start with a non‑zero digit. The parts are read
 *            consecutively from the infinite string 12345678901234567890
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 10, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *parts = NULL;      /* cached values a(1)…a(max_computed) */
static size_t parts_size = 0;    /* allocated size of parts array        */
static idx_t   max_computed = 0; /* highest index already computed      */
static idx_t   pos = 0;          /* next unused digit position           */

/* Return the digit at position i in the infinite cyclic string. */
static inline unsigned digit_at(idx_t i)
{
    unsigned r = (unsigned)(i % 10);
    return (r + 1) % 10; /* 0→1, 1→2 … 8→9, 9→0 */
}

/* Allocate or grow the parts array (C++ or C). */
static void grow_parts(size_t new_size)
{
    if (new_size <= parts_size) return;

#if defined(__cplusplus)
    seq_t *tmp = new seq_t[new_size];
    if (!tmp) return;
    for (size_t i = 0; i < parts_size; ++i) tmp[i] = parts[i];
    delete[] parts;
    parts = tmp;
#else
    seq_t *tmp = (seq_t *)realloc(parts, new_size * sizeof(seq_t));
    if (!tmp) return;
    parts = tmp;
#endif
    parts_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n < 1 */

    /* Return cached value if already computed. */
    if (n <= max_computed) return parts[n - 1];

    /* Extend cache to hold up to n. */
    grow_parts(n);
    if (parts == NULL) return 0;     /* allocation failed */

    /* Generate parts from max_computed+1 to n. */
    for (idx_t k = max_computed + 1; k <= n; ++k)
    {
        seq_t val = 0;               /* current part value   */
        uint64_t rem = 0;            /* remainder modulo k   */

        /* Skip leading zeros: a part may not start with 0. */
        while (digit_at(pos) == 0) pos++;

        bool found = false;
        /* State space: remainder (0..k-1) × position mod 10 (0..9). */
        size_t visited_sz = (size_t)k * 10;
        char *visited = (char *)calloc(visited_sz, 1);
        if (!visited) {              /* allocation failure */
            parts[k - 1] = 0;
            continue;
        }

        while (1)
        {
            unsigned d = digit_at(pos);
            pos++;
            val = val * 10 + d;
            rem = (rem * 10 + d) % (uint64_t)k;

            /* Mark state. */
            size_t state_idx = ((size_t)(pos % 10)) * (size_t)k + (size_t)rem;
            if (visited[state_idx])
            {
                /* cycle detected → no solution for this k */
                found = false;
                break;
            }
            visited[state_idx] = 1;

            if (rem == 0 && val != (seq_t)k)
            {
                /* Candidate part found: check if next digit would start with 0 */
                unsigned next_d = digit_at(pos);
                if (next_d == 0)
                {
                    /* Cannot end here; include the zero and continue. */
                    continue;
                }
                found = true;
                break;
            }
        }
        free(visited);
        parts[k - 1] = found ? val : 0;
    }

    max_computed = n;
    return parts[n - 1];
}
