/*
 * A235297  Number of (n+1) X (7+1) 0..3 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 3
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows the recurrence * a(n) =
 *            3*a(n-1)+17*a(n-2)-57*a(n-3)-86*a(n-4)+372*a(n-5) *
 *            +76*a(n-6)-972*a(n-7)+360*a(n-8)+864*a(n-9)-576*a(n-10) * for n
 *            ≥ 11, with initial values * a(1)=13376, a(2)=15692, a(3)=18832,
 *            a(4)=25592, a(5)=35936, * a(6)=58688, a(7)=97888, a(8)=187184,
 *            a(9)=358016, a(10...
 * Range:     u8 none, u16 n <= 6, u32 n <= 19, u64 n <= 40, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>    /* malloc, free */
#include <stdint.h>    /* uint64_t */

static seq_t *seq_array = NULL;
static size_t seq_cap = 0;   /* allocated capacity */
static size_t seq_len = 0;   /* next free index (so seq_len-1 is last filled) */

/* initial terms (index 0 unused) */
static const seq_t init_vals[11] = {
    0ULL,        /* unused */
    13376ULL,    /* a(1) */
    15692ULL,    /* a(2) */
    18832ULL,    /* a(3) */
    25592ULL,    /* a(4) */
    35936ULL,    /* a(5) */
    58688ULL,    /* a(6) */
    97888ULL,    /* a(7) */
    187184ULL,   /* a(8) */
    358016ULL,   /* a(9) */
    768272ULL    /* a(10) */
};

/* ensure the array has capacity for indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (n < seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap <= (size_t)n)  /* allocate at least n+1 */
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (seq_array) {
        memcpy(new_arr, seq_array, seq_len * sizeof(seq_t));
        free(seq_array);
    }
    seq_array = new_arr;
    seq_cap = new_cap;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined, but return 0 to avoid UB */
        return (seq_t)0;
    ensure_capacity(n);
    /* fill in any missing initial values */
    while (seq_len <= 10 && seq_len <= (size_t)n) {
        seq_array[seq_len] = init_vals[seq_len];
        seq_len++;
    }
    /* compute further terms using the recurrence */
    while (seq_len <= (size_t)n) {
        size_t i = seq_len;               /* compute a(i) */
        /* positive part */
        seq_t pos = (seq_t)3 * seq_array[i - 1] +
                    (seq_t)17 * seq_array[i - 2] +
                    (seq_t)372 * seq_array[i - 5] +
                    (seq_t)76 * seq_array[i - 6] +
                    (seq_t)360 * seq_array[i - 8] +
                    (seq_t)864 * seq_array[i - 9];
        /* negative part */
        seq_t neg = (seq_t)57 * seq_array[i - 3] +
                    (seq_t)86 * seq_array[i - 4] +
                    (seq_t)972 * seq_array[i - 7] +
                    (seq_t)576 * seq_array[i - 10];
        seq_array[i] = pos - neg;   /* pos ≥ neg for all n */
        seq_len++;
    }
    return seq_array[n];
}
