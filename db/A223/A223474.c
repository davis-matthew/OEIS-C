/*
 * A223474  Least positive multiple of n that when written in base 10 has
 *          digits in nonincreasing order.
 * https://oeis.org/A223474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Least positive multiple of n whose decimal digits are in
 *            non‑increasing order. Computed by a breadth‑first search over
 *            states (previous digit, remainder modulo n). The queued value is
 *            kept as a seq_t; the queue is implemented with three parallel
 *            arrays to avoid construction/destruction of seq_t o...
 * Range:     u8 n <= 33, u16 n <= 318, u32 n <= 988, u64 n <= 1088,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
/* In C++ allocate seq_t array with new[] so constructors run. */
#define ALLOC_VAL_T(n) new seq_t[(n)]
#define FREE_VAL_T(p) delete[] (p)
#else
/* In C allocate seq_t array with calloc, which is safe for trivial types. */
#define ALLOC_VAL_T(n) ((seq_t*)calloc((n), sizeof(seq_t)))
#define FREE_VAL_T(p) free(p)
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined, but safety */

    /* visited[prev * n + rem] : prev in [0..9], rem in [0..n-1] */
    size_t visited_sz = 10ULL * (size_t)n;
    unsigned char *visited = (unsigned char*)calloc(visited_sz, 1);
    if (!visited) return 0;                    /* allocation failure */

    /* Queue capacity is at most the number of possible states. */
    size_t cap = visited_sz;
    int *prev_arr = (int*)calloc(cap, sizeof(int));
    idx_t *rem_arr = (idx_t*)calloc(cap, sizeof(idx_t));
    seq_t *val_arr = ALLOC_VAL_T(cap);

    if (!prev_arr || !rem_arr || !val_arr) {
        free(prev_arr); free(rem_arr); FREE_VAL_T(val_arr); free(visited);
        return 0;
    }

    size_t head = 0, tail = 0;

    /* Enqueue all one‑digit states (1..9). */
    for (int d = 1; d <= 9; ++d) {
        idx_t r = d % n;
        size_t idx = (size_t)d * n + (size_t)r;
        if (visited[idx]) continue;             /* should not happen */
        visited[idx] = 1;
        prev_arr[tail] = d;
        rem_arr[tail] = r;
        val_arr[tail] = (seq_t)d;
        ++tail;
        if (r == 0) {
            /* clean up */
            free(prev_arr); free(rem_arr); FREE_VAL_T(val_arr); free(visited);
            return (seq_t)d;
        }
    }

    /* Breadth‑first search. */
    while (head < tail) {
        int prev = prev_arr[head];
        idx_t cur_rem = rem_arr[head];
        seq_t cur_val = val_arr[head];
        ++head;

        for (int d = 0; d <= prev; ++d) {
            idx_t r = (cur_rem * 10 + d) % n;
            size_t idx = (size_t)d * n + (size_t)r;
            if (visited[idx]) continue;
            visited[idx] = 1;

            seq_t val = cur_val * 10 + (seq_t)d;
            if (r == 0) {
                /* clean up */
                free(prev_arr); free(rem_arr); FREE_VAL_T(val_arr); free(visited);
                return val;
            }

            /* Push new state.  tail will never exceed cap. */
            prev_arr[tail] = d;
            rem_arr[tail] = r;
            val_arr[tail] = val;
            ++tail;
        }
    }

    /* In theory this point is unreachable for n >= 1. */
    free(prev_arr); free(rem_arr); FREE_VAL_T(val_arr); free(visited);
    return 0;
}
