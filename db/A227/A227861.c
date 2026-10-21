/*
 * A227861  Sum i + j for integers 2^i*3^j (A033845).
 * https://oeis.org/A227861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = i + j where 2^i * 3^j is the n-th smallest number in the
 *            increasing list of {2^i * 3^j | i>=1, j>=1}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

// static state for the incremental generator
static idx_t     seq_len = 0;          // number of terms already generated
static int      *i_arr   = NULL;       // exponents i
static int      *j_arr   = NULL;       // exponents j
static idx_t     cap_len = 0;           // allocated capacity of arrays
static idx_t     ptr2    = 0;           // pointer for next *2
static idx_t     ptr3    = 0;           // pointer for next *3

/* enlarge the exponent arrays if necessary */
static void ensure_cap(idx_t needed)
{
    if (cap_len < needed) {
        idx_t new_cap = (cap_len == 0) ? 16 : cap_len << 1;
        while (new_cap < needed) new_cap <<= 1;
        i_arr = (int *)realloc(i_arr, new_cap * sizeof(int));
        j_arr = (int *)realloc(j_arr, new_cap * sizeof(int));
        cap_len = new_cap;
    }
}

/* generate terms until we have at least n elements */
static void generate_to(idx_t n)
{
    static const double LOG2 = log(2.0);
    static const double LOG3 = log(3.0);
    const double EPS = 1e-12;   // tolerance for floating equality

    if (seq_len == 0) {                /* initialise the very first term */
        ensure_cap(16);
        i_arr[0] = 1;
        j_arr[0] = 1;
        seq_len = 1;
    }

    while (seq_len < n) {
        ensure_cap(seq_len + 1);

        /* candidate obtained by multiplying the element at ptr2 by 2 */
        double val2 = ( (double)i_arr[ptr2] + 1.0 ) * LOG2 +
                       (double)j_arr[ptr2]        * LOG3;

        /* candidate obtained by multiplying the element at ptr3 by 3 */
        double val3 = (double)i_arr[ptr3]            * LOG2 +
                       ( (double)j_arr[ptr3] + 1.0 ) * LOG3;

        if (val2 < val3 - EPS) {                     /* val2 is clearly smaller */
            i_arr[seq_len] = i_arr[ptr2] + 1;
            j_arr[seq_len] = j_arr[ptr2];
            ++ptr2;
        } else if (val3 < val2 - EPS) {              /* val3 is clearly smaller */
            i_arr[seq_len] = i_arr[ptr3];
            j_arr[seq_len] = j_arr[ptr3] + 1;
            ++ptr3;
        } else { /* the two candidates are numerically equal – skip one copy */
            i_arr[seq_len] = i_arr[ptr2] + 1;          /* either path works */
            j_arr[seq_len] = j_arr[ptr2];
            ++ptr2;
            ++ptr3;
        }
        ++seq_len;
    }
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return 0;
    generate_to(n);
    /* sum of exponents */
    return (seq_t)(i_arr[n-1] + j_arr[n-1]);
}
