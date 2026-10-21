/*
 * A055243  First differences of A001628 (Fibonacci convolution).
 * https://oeis.org/A055243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((5*n^2+37*n+50)*F(n+1)+4*(n+1)*F(n))/50 - recurrence
 *            used: a(n+4) = 2*a(n+3)+a(n+2)-2*a(n+1)-a(n)+F(n+3) with a(0)=1,
 *            a(1)=2, a(2)=6, a(3)=13 Computes Fibonacci numbers in O(n) time
 *            and uses only addition, subtraction and small multiplications,
 *            avoiding overflow for 64‑bit values that fit...
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 36, u64 n <= 79, u128 n <= 168,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#  define ALLOC_ARRAY(type, size) new type[(size)]
#  define FREE_ARRAY(ptr) delete [] (ptr)
#else
#  define ALLOC_ARRAY(type, size) ((type*)malloc((size)*sizeof(type)))
#  define FREE_ARRAY(ptr) free((ptr))
#endif

/* Fibonacci cache ----------------------------------------------------- */
static seq_t *fib_cache = NULL;
static size_t fib_cap = 0;
static size_t fib_last = 1; /* last computed index (initially 1, fib[1] exists) */

static void grow_fib(size_t required_index)
{
    if (required_index <= fib_last) return;
    size_t new_cap = fib_cap ? fib_cap : 4;
    while (new_cap <= required_index) new_cap <<= 1;
    seq_t *new_arr = (seq_t*)ALLOC_ARRAY(seq_t, new_cap);
    if (!new_arr) abort();
    for (size_t i = 0; i <= fib_last; ++i) new_arr[i] = fib_cache[i];
    if (fib_cache) FREE_ARRAY(fib_cache);
    fib_cache = new_arr;
    fib_cap   = new_cap;
    for (size_t i = fib_last + 1; i <= required_index; ++i)
        fib_cache[i] = fib_cache[i-1] + fib_cache[i-2];
    fib_last = required_index;
}

static void init_fib(void)
{
    if (!fib_cache) {
        fib_cap = 4;
        fib_cache = (seq_t*)ALLOC_ARRAY(seq_t, fib_cap);
        /* F(0)=0, F(1)=1 */
        fib_cache[0] = 0;
        fib_cache[1] = 1;
        if (fib_cap > 2) fib_cache[2] = fib_cache[1] + fib_cache[0];
        if (fib_cap > 3) fib_cache[3] = fib_cache[2] + fib_cache[1];
        fib_last = 1;
    }
}

/* a(n) cache ---------------------------------------------------------- */
static seq_t *a_cache = NULL;
static size_t a_cap = 0;
static size_t a_last = 3; /* last computed index (a[3] exists) */

static void grow_a(size_t required_index)
{
    if (required_index <= a_last) return;
    size_t new_cap = a_cap ? a_cap : 8;
    while (new_cap <= required_index) new_cap <<= 1;
    seq_t *new_arr = (seq_t*)ALLOC_ARRAY(seq_t, new_cap);
    if (!new_arr) abort();
    for (size_t i = 0; i <= a_last; ++i) new_arr[i] = a_cache[i];
    if (a_cache) FREE_ARRAY(a_cache);
    a_cache = new_arr;
    a_cap   = new_cap;

    /* Ensure Fibonacci numbers up to required_index-1 are ready */
    init_fib();
    grow_fib(required_index ? required_index - 1 : 0);

    /* Compute new a values */
    for (size_t i = a_last + 1; i <= required_index; ++i) {
        /* Term1 = 2*a[i-1] + a[i-2] + fib[i-1] */
        seq_t term1 = (seq_t)2 * a_cache[i-1] + a_cache[i-2] + fib_cache[i-1];
        /* Term2 = 2*a[i-3] + a[i-4] */
        seq_t term2 = (seq_t)2 * a_cache[i-3] + a_cache[i-4];
        a_cache[i] = term1 - term2;
    }
    a_last = required_index;
}

static void init_a(void)
{
    if (!a_cache) {
        a_cap = 8;
        a_cache = (seq_t*)ALLOC_ARRAY(seq_t, a_cap);
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)2;
        a_cache[2] = (seq_t)6;
        a_cache[3] = (seq_t)13;
        a_last = 3;
    }
}

/* Public sequence function -------------------------------------------- */
seq_t a(idx_t n)
{
    init_a();      /* ensure initial values are set */
    grow_a((size_t)n);
    return a_cache[n];
}
