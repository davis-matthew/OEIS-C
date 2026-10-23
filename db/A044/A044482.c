/*
 * A044482  Numbers k such that the string 4,4 occurs in the base-5
 *          representation of k but not of k+1.
 * https://oeis.org/A044482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 25*y - 1 where y is the n‑th positive integer * whose
 *            base‑5 representation does NOT contain the substring "44". *
 *            This definition yields all integers k such that base‑5(k)
 *            contains * 44 but base‑5(k+1) does not
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Return true iff x in base‑5 contains the consecutive digits 4,4. */
static bool has44(seq_t x)
{
    int prev = -1;
    while (x > 0) {
        int d = (int)(x % 5);
        if (prev == 4 && d == 4)
            return true;
        prev = d;
        x = x / 5;
    }
    return false;
}

#ifdef __cplusplus
/* C++ implementation using std::vector for automatic construction
 * and resizing of arbitrary‑precision seq_t objects. */
static std::vector<seq_t> cache_vec;
static idx_t next_y = 1;          /* next candidate y to examine (1‑based) */
static idx_t computed_n = 0;      /* number of terms already computed */

/* Ensure that cache_vec contains at least n terms. */
static void ensure_terms(idx_t n)
{
    while (computed_n < n) {
        /* Skip y that contain substring 44. */
        while (has44((seq_t)next_y))
            ++next_y;

        /* Compute a value: 25*y - 1. */
        seq_t val = (seq_t)next_y;
        val = val * 25 - 1;
        cache_vec.push_back(val);

        ++computed_n;
        ++next_y;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n < 1, safe return */

    ensure_terms(n);
    return cache_vec[n - 1];
}

#else
/* C implementation using manual malloc/realloc for POD seq_t types. */
static seq_t *cache = NULL;
static size_t cache_cap   = 0;
static size_t cache_size  = 0;      /* number of elements stored */
static idx_t next_y = 1;            /* next candidate y to examine */
static idx_t computed_n = 0;        /* terms computed */

static void ensure_terms(idx_t n)
{
    while (computed_n < n) {
        /* Skip y that contain substring 44. */
        while (has44((seq_t)next_y))
            ++next_y;

        /* Compute a value: 25*y - 1. */
        seq_t val = (seq_t)next_y;
        val = val * 25 - 1;

        /* Grow capacity if needed. */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 128;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                abort();          /* out of memory */
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_size++] = val;
        ++computed_n;
        ++next_y;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n < 1, safe return */

    ensure_terms(n);
    return cache[n - 1];
}
#endif
