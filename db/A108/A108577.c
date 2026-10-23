/*
 * A108577  Number of symmetry classes of 3 X 3 magic squares (with distinct
 *          positive entries) having all entries < n.
 * https://oeis.org/A108577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 151, u32 n <= 5912, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
 * a(n) = 2 a(n-1) - a(n-2) + a(n-4) - 2 a(n-5) + 2 a(n-6) - 2 a(n-7) + a(n-8)
 *        - a(n-10) + 2 a(n-11) - a(n-12) + r(n),
 * where r(n) = 1 if n = 10, 2 if n = 12, otherwise 0.
 * Initial values: a(1)=a(2)=...=a(9)=0, a(10)=1, a(11)=2, a(12)=5. */

#include <stdlib.h>
#include <string.h>

static const int D[13] = {1, -2, 1, 0, -1, 2, -2, 2, -1, 0, 1, -2, 1};

static seq_t *a_cache = NULL;
static idx_t a_size = 0; /* number of elements stored (includes index 0) */

/* Ensure that a_cache contains values up to index n (inclusive) */
static void a_ensure(idx_t n)
{
    if (a_size > n) return;                /* already enough */
    idx_t new_size = n + 1;                 /* we need indices 0..n */
    seq_t *new_arr = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_arr) return;                   /* allocation failure: leave empty */

    /* copy old data if any */
    if (a_cache) {
        memcpy(new_arr, a_cache, a_size * sizeof(seq_t));
        free(a_cache);
    }
    a_cache = new_arr;
    /* compute the newly required entries */
    for (idx_t i = a_size; i <= n; ++i) {
        if (i == 0) {                       /* a(0) is dummy 0 */
            a_cache[i] = 0;
            continue;
        }
        /* compute the sum of D[k] * a(i-k) for k=1..12 */
        long long sum = 0;
        for (int k = 1; k <= 12; ++k) {
            if (i >= (idx_t)k) {
                sum += (long long)D[k] * (long long)a_cache[i - k];
            }
        }
        long long rhs = 0;
        if (i == 10) rhs = 1;
        else if (i == 12) rhs = 2;
        long long val = -sum + rhs;
        a_cache[i] = (seq_t)val;
    }
    a_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not defined for n=0, but return 0 */
    a_ensure(n);
    return a_cache[n];
}
