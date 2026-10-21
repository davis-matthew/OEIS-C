/*
 * A079695  Values never taken by phi(j)/2 for any j: a(n) = A005277(n)/2.
 * https://oeis.org/A079695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A079695 – values never taken by φ(j)/2.
   Computed by sieving totients up to a growing bound and
   collecting all integers not equal to φ(j)/2. The first
   term is 7 (since φ(15)=8).                                   */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Forward declarations of helper functions.  */
static void add_missing(seq_t v);
static void recompute(uint64_t limit);

/* Storage for the running data.  */
static uint64_t *phi = NULL;          /* totient values up to current limit */
static size_t   phi_cap = 0;          /* capacity of 'phi' array            */
static unsigned char *seen = NULL;    /* bitmap of φ(j)/2 values            */
static size_t   seen_cap = 0;         /* capacity of 'seen'                 */

static seq_t   *miss_array = NULL;    /* collected missing values */
static size_t   miss_len   = 0;       /* number of values stored */
static size_t   miss_cap   = 0;       /* capacity of miss_array          */

static size_t   next_candidate = 1;   /* next m to test for missingness */
static uint64_t current_limit = 0;    /* current maximum j processed      */

/* Append a new missing value to 'miss_array'.   */
static void add_missing(seq_t v)
{
    if (miss_len == miss_cap) {
        size_t new_cap = miss_cap ? miss_cap * 2 : 256;
        seq_t *tmp = (seq_t *)realloc(miss_array, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        miss_array = tmp;
        miss_cap   = new_cap;
    }
    miss_array[miss_len++] = v;
}

/* Compute totients up to 'limit' and mark all φ(j)/2 values.   */
static void recompute(uint64_t limit)
{
    /* Ensure 'phi' has enough space.  */
    if (phi_cap < (size_t)(limit + 1)) {
        uint64_t *tmp = (uint64_t *)realloc(phi, (size_t)(limit + 1) * sizeof(uint64_t));
        if (!tmp) abort();
        phi = tmp;
        phi_cap = (size_t)(limit + 1);
    }

    /* Initialize phi[i] = i.  */
    for (uint64_t i = 0; i <= limit; ++i) phi[i] = i;
    phi[1] = 1;                 /* φ(1) = 1 */

    /* Sieve totients.  */
    for (uint64_t i = 2; i <= limit; ++i) {
        if (phi[i] == i) {      /* i is prime */
            for (uint64_t j = i; j <= limit; j += i)
                phi[j] -= phi[j] / i;
        }
    }

    /* Prepare bitmap 'seen' for φ(j)/2 values.  */
    size_t seen_size = limit / 2 + 1;
    if (seen_cap < seen_size) {
        unsigned char *tmp = (unsigned char *)realloc(seen, seen_size);
        if (!tmp) abort();
        seen = tmp;
        seen_cap = seen_size;
    }
    memset(seen, 0, seen_size);

    for (uint64_t i = 2; i <= limit; ++i) {
        uint64_t v = phi[i];
        if ((v & 1ULL) == 0) {      /* φ(i) even */
            size_t h = (size_t)(v / 2);
            if (h < seen_size) seen[h] = 1;
        }
    }
}

/* Return the nth term of the sequence (1‑based index).  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset is 1 */

    while (miss_len < (size_t)n) {
        if (current_limit == 0) {            /* initial sweep */
            current_limit = 200000;          /* ample starting bound */
            recompute(current_limit);
            size_t half = current_limit / 2;
            for (size_t i = next_candidate; i <= half; ++i)
                if (!seen[i]) add_missing((seq_t)i);
            next_candidate = half + 1;
        } else {                              /* need more values */
            current_limit *= 2;
            recompute(current_limit);
            size_t half = current_limit / 2;
            for (size_t i = next_candidate; i <= half; ++i)
                if (!seen[i]) add_missing((seq_t)i);
            next_candidate = half + 1;
        }
    }
    return miss_array[(size_t)n - 1];
}
