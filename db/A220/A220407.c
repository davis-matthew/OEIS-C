/*
 * A220407  Equals two maps: number of 2 X n binary arrays indicating the
 *          locations of corresponding elements equal to exactly two of their
 *          horizontal, diagonal and antidiagonal neighbors in a random 0..1 2
 *          X n array.
 * https://oeis.org/A220407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 7*a(n-2) - 14*a(n-3) - 19*a(n-4) + 38*a(n-5) +
 *            25*a(n-6) - 50*a(n-7) - 16*a(n-8) + 32*a(n-9) + 4*a(n-10) -
 *            8*a(n-11) for n>=12 initial values: a(1)=1, a(2)=6, a(3)=17,
 *            a(4)=40, a(5)=93, a(6)=206, a(7)=431, a(8)=924, a(9)=1863,
 *            a(10)=3898, a(11)=7773
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation keeps a memoised table that grows as necessary.
    It always stores all values up to the largest n requested so far,
    making subsequent calls constant‑time for already‑computed indices.   */
#include <stdlib.h>

/* Initial 11 terms of the sequence (index 0 is unused) */
static const seq_t init[12] = {
    0ULL,   /* unused index 0 */
    1ULL,   /* a(1)  */
    6ULL,   /* a(2)  */
    17ULL,  /* a(3)  */
    40ULL,  /* a(4)  */
    93ULL,  /* a(5)  */
    206ULL, /* a(6)  */
    431ULL, /* a(7)  */
    924ULL, /* a(8)  */
    1863ULL,/* a(9)  */
    3898ULL,/* a(10) */
    7773ULL /* a(11) */
};

/* Recurrence coefficients (from the OEIS recurrence) */
static const int coeff[11] = {
    2,   7,  -14, -19, 38, 25, -50, -16, 32, 4, -8
};

seq_t a(idx_t n) {
    /* Persistent cache: arr[i] holds a(i), arr_index is next free index */
    static seq_t *arr = NULL;
    static idx_t arr_capacity = 0;   /* size of allocated array */
    static idx_t arr_index = 0;      /* first unused index: arr[0..arr_index-1] valid */

    /* Basic sanity: offset 1, so n>=1 is relevant */
    if (n < 1) return 0;   /* undefined for non‑positive indices */

    /* Allocate or grow the cache if required */
    if (arr == NULL) {
        /* Start with capacity sufficient for the first 12 values (indices 0..11) */
        arr_capacity = 12;
        arr = (seq_t*)malloc((size_t)arr_capacity * sizeof(seq_t));
        if (!arr) return 0;                 /* allocation failure */
        arr_index = 0;
    } else if (n >= arr_capacity) {
        /* Need space for indices 0..n */
        size_t new_cap = (size_t)n + 1;
        if (new_cap < arr_capacity) new_cap = arr_capacity * 2; /* guard against overflow */
        seq_t *tmp = (seq_t*)realloc(arr, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failure */
        arr = tmp;
        arr_capacity = (idx_t)new_cap;
    }

    /* Initialise the first 11 terms if not done yet */
    if (arr_index < 12) {
        for (idx_t i = 1; i <= 11; ++i) {
            arr[i] = init[i];
        }
        arr_index = 12;   /* next free index after the known terms */
    }

    /* If the requested value is already cached, return it */
    if (n < arr_index) {
        return arr[n];
    }

    /* Compute terms from arr_index up to n using the recurrence */
    for (idx_t k = arr_index; k <= n; ++k) {
        seq_t term = 0;
        for (int j = 0; j < 11; ++j) {
            seq_t prev = arr[k - (idx_t)(j + 1)];
            int c = coeff[j];
            /* Multiply by the absolute coefficient first */
            seq_t prod = (seq_t)(c >= 0 ? c : -c) * prev;
            if (c >= 0) {
                term += prod;
            } else {
                term -= prod;
            }
        }
        arr[k] = term;
    }

    arr_index = n + 1;   /* all values up to n are now valid */
    return arr[n];
}
